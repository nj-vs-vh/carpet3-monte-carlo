from dataclasses import dataclass
from io import BytesIO
import os
from pathlib import Path
from random import random
import struct
import subprocess

from typing import List


@dataclass
class Particle:
    id: int
    px: float
    py: float
    pz: float
    x: float
    y: float
    t: float

    def pack(self) -> bytes:
        return struct.pack(
            "iffffff", self.id, self.px, self.py, self.pz, self.x, self.y, self.t
        )


def spawn_muons(n: int) -> List[Particle]:
    particles: List[Particle] = []
    for _ in range(n):
        particles.append(
            Particle(
                id=5 + round(random()),  # mu+ or mu-
                px=0.0,
                py=0.0,
                pz=4 + 6 * random(),  # 4 to 10 GeV
                x=100 * (-5 + 10 * random()),
                y=100 * (-5 + 10 * random()),
                t=0.0,
            )
        )
    return particles


CUR_DIR = Path(__file__).parent
ROOT_DIR = CUR_DIR / ".."
DATA_DIR = (ROOT_DIR / "data").resolve()
DATA_DIR.mkdir(exist_ok=True)
EXEC_DIR = Path(os.environ["CARPET3_BIN_DIR"])

def run():
    n_muons = 100

    output_file = DATA_DIR / "example-output.txt"

    input_file = DATA_DIR / "example-input.prtcls"
    input_file_contents = struct.pack("i", n_muons) + b"".join(p.pack() for p in spawn_muons(n_muons))
    input_file.write_bytes(input_file_contents)


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


run()
