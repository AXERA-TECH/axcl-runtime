#!/usr/bin/env python3
import re
import sys


def remove_function_declarations(content):
    """
    Remove function declarations from C header file content while preserving:
    - Struct definitions (including function pointers inside structs)
    - Type definitions (all typedef content)
    - Comments
    - Other non-function declaration content
    """
    lines = content.split('\n')
    result_lines = []

    i = 0
    n = len(lines)

    while i < n:
        line = lines[i]
        stripped_line = line.strip()

        # Skip empty lines and comments
        if (not stripped_line or
                stripped_line.startswith('//') or
                stripped_line.startswith('/*')):
            result_lines.append(line)
            i += 1
            continue

        '''
        skip struct, union, enum definition:
            struct T {
                void (*func)(void);
            }
        '''
        if (stripped_line.startswith('struct ') or
                stripped_line.startswith('union ') or
                stripped_line.startswith('enum ')):
            result_lines.append(line)
            i += 1
            continue

        '''
        skip typedef definition:
            typedef void (*func)(void);
        '''
        if stripped_line.startswith('typedef'):
            result_lines.append(line)
            i += 1
            continue

        # Directly check if it's a function declaration
        if (re.match(
            r'^[A-Za-z_][A-Za-z0-9_]*\s+[A-Za-z_][A-Za-z0-9_]*\s*\(',
            stripped_line
        ) or re.match(
            r'^[A-Za-z_][A-Za-z0-9_]*\s*\*\s*[A-Za-z_][A-Za-z0-9_]*\s*\(',
            stripped_line
        )):
            # If it's a function declaration, skip until semicolon is found
            found_semicolon = ';' in stripped_line
            j = i + 1

            while j < n and not found_semicolon:
                next_line = lines[j]
                if ';' in next_line.strip():
                    found_semicolon = True
                j += 1

            i = j
        else:
            result_lines.append(line)
            i += 1

    return '\n'.join(result_lines)


def process_header_file(input_file, output_file=None):
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            content = f.read()

        data = remove_function_declarations(content)

        if output_file:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(data)
        else:
            print(data)

    except FileNotFoundError:
        print(f"Error: File not found {input_file}")
    except Exception as e:
        print(f"Error processing file: {e}")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage  : python remove_func.py <input_header_file> [output_file]")
        print("Example: python remove_func.py input.h output.h")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else None

    process_header_file(input_file, output_file)