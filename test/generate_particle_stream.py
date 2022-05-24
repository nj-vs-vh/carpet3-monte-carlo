from dataclasses import dataclass
import struct
import sys
from typing import List
from random import random
from pathlib import Path


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


def generate_particle_stream(path: Path, n_muons: int) -> bytes:
    print(f"Generating fake particle stream with params: {path=!r} {n_muons=!r}")
    content = struct.pack("i", n_muons) + b"".join(
        p.pack() for p in spawn_muons(n_muons)
    )
    path.write_bytes(content)
    print(f"Done!")
    return content


if __name__ == "__main__":
    with open(snakemake.log[0], "w") as log:  # type: ignore
        sys.stdout = log
        sys.stderr = log
        generate_particle_stream(Path(snakemake.output[0]), snakemake.params["n_muons"])  # type: ignore
