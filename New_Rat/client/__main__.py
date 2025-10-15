from .controller import CmdController
import sys

def main():
    controller = CmdController()

    try:
        controller.cmdloop()
    except KeyboardInterrupt:
        print("\nExiting program...")
        sys.exit(0)

if __name__ == "__main__":
    main()