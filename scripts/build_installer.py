#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
AXCL Installer Builder
Compiles NSIS installer with specified files and version

Usage:
    python build_installer.py [zip_path] [pac_path] [version]
    
Arguments:
    zip_path: Path to AXCL zip file (optional, defaults to axcl.win64.zip)
    pac_path: Path to AX650 pac file (required)
    version:  Version string (optional, defaults to 1.0.0)
"""

import os
import subprocess
import sys
import shutil
import zipfile
import argparse
import tempfile

def check_nsis():
    """Check if NSIS is installed and available"""
    try:
        result = subprocess.run(['makensis', '/VERSION'], 
                              capture_output=True, text=True, check=True)
        print(f"NSIS found: {result.stdout.strip()}")
        return True
    except (subprocess.CalledProcessError, FileNotFoundError):
        print("Error: NSIS compiler (makensis.exe) not found")
        print("Please install NSIS from: https://nsis.sourceforge.io/Download")
        print("Or ensure makensis.exe is in your PATH")
        return False

def extract_zip_file(zip_file):
    """Extract zip file to a temporary directory in system temp folder"""
    # Create a temporary directory in system temp folder
    temp_dir = tempfile.mkdtemp(prefix="axcl_", suffix="_extracted")
    extract_to = os.path.join(temp_dir, "axcl_extracted")
    
    print(f"Extracting {zip_file} to {extract_to}...")
    
    # Create extraction directory
    os.makedirs(extract_to, exist_ok=True)
    
    # Extract zip file
    with zipfile.ZipFile(zip_file, 'r') as zip_ref:
        zip_ref.extractall(extract_to)
    
    print(f"Successfully extracted to {extract_to}")
    return extract_to, temp_dir


def parse_arguments():
    """Parse command line arguments"""
    parser = argparse.ArgumentParser(
        description='AXCL Installer Builder - Build NSIS installer for AXCL',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python build_installer.py axcl.win64.zip AX650_card_V3.6.5.pac
  python build_installer.py axcl.win64.zip AX650_card_V3.6.5.pac 1.0.0
        """
    )
    
    parser.add_argument(
        'zip_path',
        nargs='?',
        default='axcl.win64.zip',
        help='Path to AXCL zip file (default: axcl.win64.zip)'
    )
    
    parser.add_argument(
        'pac_path',
        help='Path to AX650 pac file (required)'
    )
    
    parser.add_argument(
        'version',
        nargs='?',
        default='1.0.0',
        help='Version string (default: 1.0.0)'
    )
    
    return parser.parse_args()


def main():
    """Main function"""
    print("=" * 60)
    print("AXCL Installer Builder")
    print("=" * 60)
    
    # Parse command line arguments
    args = parse_arguments()
    
    zip_file = args.zip_path
    pac_file = args.pac_path
    version = args.version
    
    print("Configuration:")
    print(f"  ZIP file: {zip_file}")
    print(f"  PAC file: {pac_file}")
    print(f"  Version:  {version}")
    print()
    
    # Check if we're in the right directory
    if not os.path.exists("install_axcl.nsi"):
        print("Error: install_axcl.nsi not found in current directory")
        print("Please run this script from the directory containing the NSIS script")
        return 1
    
    # Check required files
    required_files = [pac_file, zip_file]
    for file in required_files:
        if not os.path.exists(file):
            print(f"Error: {file} not found")
            return 1
    
    print("All required files found")
    
    # Extract zip file to temp directory
    extract_dir, temp_dir = extract_zip_file(zip_file)
    
    # Check NSIS installation
    if not check_nsis():
        # Clean up temp directory
        shutil.rmtree(temp_dir, ignore_errors=True)
        return 1
    

    
    # Compile with NSIS
    try:
        print("\nCompiling with NSIS...")
        print(f"  PAC file: {pac_file}")
        print(f"  Version:  {version}")
        
        # Convert version to 4-part format for VIProductVersion if needed
        version_parts = version.split('.')
        if len(version_parts) == 3:
            vi_version = f"{version}.0"
        elif len(version_parts) == 4:
            vi_version = version
        else:
            print(f"Warning: Unusual version format '{version}', using as-is")
            vi_version = f"{version}.0.0.0"[:len(version)+4]  # Ensure 4 parts
        
        print(f"  VI Version: {vi_version}")
        print(f"  Extract Dir: {extract_dir}")
        
        # Pass the PAC file, version, and extract directory to NSIS as compile-time defines
        subprocess.run([
            "makensis", 
            f"/DDRIVE_PAC_FILE={pac_file}",
            f"/DPRODUCT_VERSION={version}",
            f"/DVI_PRODUCT_VERSION={vi_version}",
            f"/DAXCL_EXTRACTED_DIR={extract_dir}",
            "install_axcl.nsi"
        ], check=True, capture_output=True, text=True)
        print("Compilation successful!")
        
        # Check if installer was created
        if os.path.exists("AXCL_Setup.exe"):
            file_size = os.path.getsize("AXCL_Setup.exe")
            print(f"Installer created: AXCL_Setup.exe ({file_size:,} bytes)")
        else:
            print("Warning: Installer file not found after compilation")
        
        print("\n" + "=" * 60)
        print("Build completed successfully!")
        print("You can now distribute AXCL_Setup.exe")
        print("=" * 60)
        
        # Clean up temp directory
        print(f"\nCleaning up temporary directory: {temp_dir}")
        shutil.rmtree(temp_dir, ignore_errors=True)
        
        return 0
        
    except subprocess.CalledProcessError as e:
        print("Compilation failed!")
        if e.stderr:
            print(f"Error output: {e.stderr}")
        if e.stdout:
            print(f"Output: {e.stdout}")
        # Clean up temp directory
        shutil.rmtree(temp_dir, ignore_errors=True)
        return 1
    except FileNotFoundError:
        print("Error: NSIS (makensis) not found in PATH")
        # Clean up temp directory
        shutil.rmtree(temp_dir, ignore_errors=True)
        return 1

if __name__ == "__main__":
    sys.exit(main())