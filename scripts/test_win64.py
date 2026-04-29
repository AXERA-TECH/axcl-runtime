#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import locale
import os
import signal
import subprocess
import sys
import time
from datetime import datetime
from typing import Any, Dict, List


DEFAULT_BIN_DIR = "../out/axcl_win_x64/bin"
DEFAULT_CFG_FILE = "test_win64.json"
SPAN_WIDTH = 80


class TestRunner:
    def __init__(self, config_file: str = DEFAULT_CFG_FILE):
        self.config_file = config_file
        self.cases = []
        self.results = []
        self.log_file = f"test_log_{datetime.now().strftime('%Y%m%d%H%M%S')}.txt"
        self.system_encoding = self._get_system_encoding()
        self.bin_dir = DEFAULT_BIN_DIR
        self.env = {}

    def _get_system_encoding(self) -> str:
        try:
            encoding = locale.getpreferredencoding()
            if encoding.lower() in ['cp936', 'gbk', 'gb2312']:
                return 'gbk'
            elif encoding.lower() in ['cp1252', 'windows-1252']:
                return 'cp1252'
            else:
                return 'utf-8'
        except Exception:
            return 'gbk'

    def load_config(self) -> bool:
        try:
            if not os.path.exists(self.config_file):
                print(f"{self.config_file} not found")
                return False

            with open(self.config_file, 'r', encoding='utf-8') as f:
                config = json.load(f)

            cases = config.get('cases', [])
            if not cases:
                print(f"No cases found in {self.config_file}")
                return False

            self.cases = [case for case in cases if case.get('enable', 1) == 1]
            self.bin_dir = os.path.abspath(config.get('bin_dir', DEFAULT_BIN_DIR))
            self.env = config.get('env', '')
            return True

        except Exception as e:
            print(f"Failed to load {self.config_file}: {e}")
            return False

    def run_case(self, case: Dict[str, Any], index: int) -> Dict[str, Any]:
        name = case.get('name', f'Case{index}')
        bin = case.get('bin', '')
        args = case.get('args', [])
        pass_keywords = case.get('pass_keywords', [])
        fail_keywords = case.get('fail_keywords', [])
        fail_keywords.append('[E]')
        fail_keywords.append('FAIL')
        timeout = case.get('timeout', 30)
        retries = max(case.get('retry', 1), 1)

        if not os.path.isabs(bin):
            bin = os.path.join(self.bin_dir, bin)
        bin = os.path.abspath(bin)

        print(f"\n[{index:02d}/{len(self.cases):02d}] {name}")
        print(f"Command: {bin} {' '.join(args)}")
        if self.env:
            print(f"Environment: {self.env}")

        result = {
            'name': name,
            'bin': bin,
            'args': args,
            'index': index,
            'status': 'UNKNOWN',
            'output': '',
            'error': '',
            'elapse': 0,
            'timestamp': datetime.now().isoformat()
        }

        for retry in range(retries):
            try:
                result = self._run_once(bin, args, timeout,
                                       pass_keywords, fail_keywords, result)
                if result['status'] != 'FAIL':
                    break
                elif retry < retries - 1:
                    print(f"Retrying {retry}/{retries-1}...")
                    time.sleep(1)

            except Exception as e:
                result['status'] = 'FAIL'
                result['error'] = str(e)
                print(f"Failed: {result['error']}")
                if retry < retries - 1:
                    print(f"Exception occurred, retrying...")
                    time.sleep(1)

        return result

    def _run_once(self, bin: str, args: List[str], timeout: int,
                  pass_keywords: List[str], fail_keywords: List[str],
                  result: Dict[str, Any]) -> Dict[str, Any]:

        try:
            if not os.path.exists(bin):
                result['status'] = 'FAIL'
                result['elapse'] = 0
                result['error'] = f"bin not found: {bin}"
                print(f"Status: FAIL")
                print(f"Failed: {result['error']}")
                return result

            run_env = os.environ.copy()

            # Add bin_dir to PATH
            current_path = run_env.get('PATH', '')
            if self.bin_dir not in current_path:
                run_env['PATH'] = f"{self.bin_dir};{current_path}"

            # Apply custom env configuration
            if self.env:
                if '%PATH%' in self.env:
                    new_path = self.env.replace('%PATH%',
                                               run_env.get('PATH', ''))
                else:
                    new_path = self.env
                run_env['PATH'] = new_path

            start_time = time.time()
            process = subprocess.Popen(
                [bin] + args,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                encoding=self.system_encoding,
                errors='replace',
                env=run_env
            )

            try:
                stdout, stderr = process.communicate(timeout=timeout)
                elapse = time.time() - start_time
                result['elapse'] = elapse

                output = stdout + stderr
                result['output'] = output

                result['status'] = self._check_result(
                    output, pass_keywords, fail_keywords)

                print(f"Elapse: {elapse:.2f}s")
                print(f"Status: {result['status']}")
                print("-" * SPAN_WIDTH)

            except subprocess.TimeoutExpired:
                try:
                    for _ in range(3):
                        process.send_signal(signal.CTRL_C_EVENT)
                        time.sleep(1)
                except Exception:
                    pass

                try:
                    process.kill()
                except Exception:
                    pass

                result['status'] = 'FAIL'
                result['error'] = f"Execution timeout ({timeout}s)"
                result['elapse'] = timeout
                print(f"Failed: {result['error']}")

        except Exception as e:
            result['status'] = 'FAIL'
            result['error'] = str(e)
            print(f"Failed: {result['error']}")

        return result

    def _check_result(self, output: str, pass_keywords: List[str],
                      fail_keywords: List[str]) -> str:
        for keyword in fail_keywords:
            if keyword in output:
                return 'FAIL'

        if not pass_keywords:
            return 'PASS'

        for keyword in pass_keywords:
            if keyword not in output:
                return 'UNKNOWN'
        return 'PASS'

    def run(self):
        if not self.load_config():
            return

        print(f"Total {len(self.cases)} cases loaded from {self.config_file}")
        print(f"Encoding: {self.system_encoding}")
        print(f"Bin directory: {self.bin_dir}")
        print("=" * SPAN_WIDTH)

        for i, case in enumerate(self.cases, 1):
            result = self.run_case(case, i)
            self.results.append(result)
            self._log_result(result)

        self._show_statistics()

    def _log_result(self, result: Dict[str, Any]):
        try:
            with open(self.log_file, 'a', encoding='utf-8') as f:
                f.write( f"Case [{result['index']}/{len(self.cases)}]: \n")
                f.write(f"Name  : {result['name']}\n")
                f.write(f"Bin   : {result['bin']}\n")
                f.write(f"Args  : {result['args']}\n")
                f.write(f"Status: {result['status']}\n")
                f.write(f"Elapse: {result['elapse']:.2f}s\n")
                if result['error']:
                    f.write(f"Error : {result['error']}\n")
                f.write("Output:\n")
                f.write(result['output'])
                f.write("\n\n" + "=" * SPAN_WIDTH + "\n\n")
        except Exception as e:
            print(f"Failed to write log to {self.log_file}: {e}")

    def _show_statistics(self):
        print("\n" + "=" * SPAN_WIDTH)
        print("SUMMARY".center(SPAN_WIDTH))
        print("=" * SPAN_WIDTH)

        totals = len(self.results)
        passed = len([r for r in self.results if r['status'] == 'PASS'])
        failed = len([r for r in self.results if r['status'] == 'FAIL'])
        unknown = len([r for r in self.results if r['status'] == 'UNKNOWN'])

        print(f"TOTAL  : {totals:>3}")
        print(f"PASS   : {passed:>3}")
        print(f"FAIL   : {failed:>3}")
        print(f"UNKNOWN: {unknown:>3}")
        print(f"RATE   : {(passed/totals*100):>5.1f}%")

        print("\nDETAIL:")
        print("-" * SPAN_WIDTH)
        print(f"{'#':<4} {'Case Name':<25} {'Status':<8} {'Time(s)':<8} "
              f"{'Description'}")
        print("-" * SPAN_WIDTH)

        for result in self.results:
            status_text = result['status']
            remark = result['error'] if result['error'] else ""
            name = result['name']
            if len(name) > 25:
                name = name[:22] + "..."

            print(f"{result['index']:<4} {name:<25} {status_text:<8} "
                  f"{result['elapse']:<8.2f} {remark}")

        print(f"\nLog saved to: {self.log_file}")


def main():
    print("=" * SPAN_WIDTH)
    print("Windows Test Runner".center(SPAN_WIDTH))
    print("=" * SPAN_WIDTH)

    config_file = DEFAULT_CFG_FILE
    if len(sys.argv) > 1:
        config_file = sys.argv[1]

    runner = TestRunner(config_file)
    runner.run()


if __name__ == "__main__":
    main()
