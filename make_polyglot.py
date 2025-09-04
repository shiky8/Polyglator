import sys
from pathlib import Path

def make_polyglot(exe_path, png_path, out_path="polyglot.exe"):
    exe_data = Path(exe_path).read_bytes()
    png_data = Path(png_path).read_bytes()

    # Polyglot = [EXE header + EXE body] + [PNG header + PNG data]
    with open(out_path, "wb") as f:
        f.write(exe_data)
        f.write(png_data)

    print(f"[+] Polyglot created: {out_path}")
    print("    - Run as .exe → executes program")
    print("    - After header rewrite → rename to .png → image opens")


if __name__ == "__main__":
    # make_polyglot("myprog.exe", "myimage.png", "polyglot.exe")
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <exe> <png> <output>")
        sys.exit(1)

    make_polyglot(sys.argv[1], sys.argv[2], sys.argv[3])
