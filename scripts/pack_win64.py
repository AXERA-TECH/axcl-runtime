#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
import zipfile
from pathlib import Path

PACK_DIR_LIST = [
    '3rdparty',
    'build',
    'cmake',
    'logger',
    'sample',
    'out',
    'toolkit',
    'scripts',
]

IGNORE_LIST = [
    '3rdparty/ffmpeg/n7.1.tar.gz',
    '3rdparty/ffmpeg/FFmpeg-n7.1',
    '3rdparty/ffmpeg/arm64',
    '3rdparty/ffmpeg/x64',
    '3rdparty/ffmpeg/riscv',
    '3rdparty/ffmpeg/build.sh',
    '3rdparty/googletest/arm64',
    '3rdparty/googletest/x64',
    '3rdparty/googletest/riscv',
    '3rdparty/googletest/scripts',
    '3rdparty/protobuf/arm64',
    '3rdparty/protobuf/x64',
    '3rdparty/protobuf/riscv',
    '3rdparty/protobuf/scripts',
    '3rdparty/spdlog/arm64',
    '3rdparty/spdlog/x64',
    '3rdparty/spdlog/riscv',
    '3rdparty/spdlog/scripts',
    '3rdparty/zmq',

    'build/package',
    'build/projects',

    'cmake/device',
    'cmake/host',
    'cmake/driver',
    'cmake/protocol',
    'cmake/test',
    'cmake/sample/aicard.cmake',
    'cmake/sample/x86app.cmake',
    'cmake/sample/subid.cmake',
    'cmake/sample/skel.cmake',

    'sample/aicard',
    'sample/ive',
    'sample/ivps',
    'sample/skel',
    'sample/subid',
    'sample/x86app',
    'sample/venc',

    'scripts/pack_win64.py',
]

PACK_NAME = 'axcl.win64.zip'


def rm(path):
    if path.exists():
        if path.is_file():
            path.unlink()
        elif path.is_dir():
            shutil.rmtree(path)


def parse_args():
    parser = argparse.ArgumentParser(
        description=f'Script to create {PACK_NAME}',
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument(
        '-o', '--output',
        default='.',
        help=f'output directory for {PACK_NAME} (default: .)'
    )

    return parser.parse_args()


def pack(output_dir):
    script_dir = Path(__file__).parent
    root_dir = script_dir.parent
    output_path = Path(output_dir).resolve()
    if not output_path.exists():
        try:
            output_path.mkdir(parents=True, exist_ok=True)
        except Exception as e:
            print(f"Error: Failed to create output directory: {e}")
            return 1

    # check if all directories exist
    missing = []
    for dir_name in PACK_DIR_LIST:
        src_path = root_dir / dir_name
        if not src_path.exists():
            missing.append(dir_name)
    if missing:
        print(f"Error: The following directories do not exist: {', '.join(missing)}")
        return 1

    # Clean directories with git clean (except 'out' directory)
    dirs_to_clean = [d for d in PACK_DIR_LIST if d != 'out']
    for dir_name in dirs_to_clean:
        dir_path = root_dir / dir_name
        subprocess.run(
            ['git', 'clean', '-xfd'],
            cwd=dir_path,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=60
        )

    # Create temporary directory
    temp_dir_obj = tempfile.TemporaryDirectory()
    temp_path = Path(temp_dir_obj.name)
    temp_dir = temp_path / 'axcl'

    try:
        temp_dir.mkdir(exist_ok=True)

        for dir_name in PACK_DIR_LIST:
            src_path = root_dir / dir_name
            dst_path = temp_dir / dir_name

            print(f"Copying {dir_name}/ -> {dst_path}")
            try:
                shutil.copytree(src_path, dst_path)
            except Exception as e:
                print(f"Failed to copy {dir_name}/: {e}")
                return 1

        # remove ignore list
        for item in IGNORE_LIST:
            file_path = temp_dir / item
            rm(file_path)

        # delete all Makefile
        for root, dirs, files in os.walk(temp_dir):
            for file in files:
                if file == 'Makefile':
                    rm(Path(root) / file)

        # rm uncompiled directory in CMakeLists.txt
        file_path = temp_dir / 'cmake' / 'CMakeLists.txt'
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        subdir_list = ['driver', 'protocol', 'host', 'test', 'device']
        for item in subdir_list:
            pattern = rf'^\s*add_subdirectory\s*\(\s*{item}\s*\)$'
            content = re.sub(
                pattern,
                f'# add_subdirectory ({item})',
                content,
                flags=re.MULTILINE
            )
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(content)

        # replace axcl::xxx to ${AXCL_INSTALL_LIBRARY_PATH}/libaxcl_xxx.lib
        for root, dirs, files in os.walk(temp_dir / 'cmake' / 'sample'):
            for file in files:
                file_path = Path(root) / file
                with open(file_path, 'r', encoding='utf-8') as f:
                    content = f.read()

                replaced = {
                    'axcl::host::comm': 'libaxcl_comm.lib',
                    'axcl::runtime': 'libaxcl_rt.lib',
                    'axcl::native::sys': 'libaxcl_sys.lib',
                    'axcl::native::ivps': 'libaxcl_ivps.lib',
                    'axcl::native::vdec': 'libaxcl_vdec.lib',
                    'axcl::native::venc': 'libaxcl_venc.lib',
                    'axcl::native::dmadim': 'libaxcl_dmadim.lib',
                    'axcl::native::npu': 'libaxcl_npu.lib',
                    'axcl::native::dsp': 'libaxcl_dsp.lib',
                    'axcl::native::ive': 'libaxcl_ive.lib',
                }

                for k, v in replaced.items():
                    content = content.replace(k, '${AXCL_INSTALL_LIBRARY_PATH}/' + v)

                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(content)

        zip_path = output_path / PACK_NAME
        print(f"Packing {zip_path} ...")
        try:
            with zipfile.ZipFile(zip_path, 'w', zipfile.ZIP_DEFLATED) as zipf:
                for root, dirs, files in os.walk(temp_dir):
                    for file in files:
                        file_path = Path(root) / file
                        arc_path = file_path.relative_to(temp_path)
                        zipf.write(file_path, arc_path)

        except Exception as e:
            print(f"Failed to create {zip_path}: {e}")
            return 1

    finally:
        temp_dir_obj.cleanup()

    print("Completed!")
    return 0


if __name__ == '__main__':
    args = parse_args()
    sys.exit(pack(args.output))
