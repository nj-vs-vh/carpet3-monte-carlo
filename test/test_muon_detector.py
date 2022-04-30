from dataclasses import dataclass
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
        return struct.pack("iffffff", self.id, self.px, self.py, self.pz, self.x, self.y, self.t)



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
EXEC_DIR = Path(os.environ["CARPET3_BIN_DIR"])

n = 100

input_file = DATA_DIR / "example-input.prtcls"
with open(DATA_DIR / input_file, "wb") as f:
    for p in spawn_muons(n):
        f.write(p.pack())

output_file = DATA_DIR / "example-output.txt"
subprocess.run(
    [
        str(EXEC_DIR / "MuonDetector"),
        str(n),
        str(input_file),
        str(output_file),
    ]
)
