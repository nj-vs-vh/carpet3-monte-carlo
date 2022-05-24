import os
from pathlib import Path
import subprocess

from test.generate_particle_stream import generate_particle_stream


CUR_DIR = Path(__file__).parent
ROOT_DIR = CUR_DIR / ".."
DATA_DIR = (ROOT_DIR / "data").resolve()
DATA_DIR.mkdir(exist_ok=True)
EXEC_DIR = Path(os.environ["CARPET3_BIN_DIR"])


def run():
    n_muons = 100
    output_file = DATA_DIR / "example-output.txt"
    input_file = DATA_DIR / "example-input.prtcls"
    input_file_contents = generate_particle_stream(input_file, n_muons)

    def run_muon_detector(stdin: bool, verbose: bool):
        cmd = [
            str(EXEC_DIR / "MuonDetector"),
            str(input_file) if not stdin else "-",
            str(output_file),
        ]
        if verbose:
            cmd.append("--verbose")

        p = subprocess.Popen(cmd, stdin=subprocess.PIPE)
        p.communicate(input=input_file_contents if stdin else None)

    run_muon_detector(stdin=False, verbose=False)
    run_muon_detector(stdin=False, verbose=True)
    run_muon_detector(stdin=True, verbose=False)


if __name__ == "__main__":
    run()
