# File formats used throughout the package

## Particle stream

Simplistic format to store or transmit a bunch of particles (usually previously simulated by the CORSIKA).
Consists of a stream of 28-byte structs, encoding information about a single particle.

The fields are:
- `id`, integer, given according to CORSIKA id system (see [Guide](https://web.iap.kit.edu/corsika/usersguide/usersguide.pdf), table at p. 120)
- momentum `p`, 3 floating point numbers for each component, given in **GeV/c**
- position on observation level `x`, `y`, floating point numbers, given in **cm**
  - ⚠️ may be given in any coordinate system, depending on context
- arrival time `t` at observation level, floating point number in **nsec**
  - ⚠️ may be given relative to any moment in time, depending on context 

### C++ [struct](../src/detector/include/C2Primary.hh):

```C++
struct Particle
{
    int id;
    float p[3];
    float x;
    float y;
    float t;
};
```

### Python [dataclass](../test/test_muon_detector.py):

```python
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
```
