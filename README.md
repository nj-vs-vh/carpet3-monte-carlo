# Carpet-3 Monte-Carlo

## Installation

The Monte-Carlo simulation requires external dependencies and a build step.
The configuration and parameters for all these steps is stored in [`.env`](https://www.dotenv.org/) file.
Make sure to create your own private (not tracked in git) copy of it with

```bash
cp .env.example .env
```

### Prerequisites

#### GEANT4
See [intallation guide](https://indico.cern.ch/event/679723/contributions/2792554/attachments/1559217/2453759/Geant4InstallationGuide.pdf)
for different options on different platforms.

For Geant 11.0.1 installation on Ubuntu 20.04 the process is as follows:
- Specify the correct version in `.env` file under `G4_VERSION`
- Download the [source code](http://cern.ch/geant4-data/releases/geant4-v11.0.1.tar.gz)
- Unpack the TAR arhchive: `tar -xzf geant4-v11.0.1.tar.gz`
- Make sure you have `cmake` and `gcc` with decently modern versions (see guide for exact requirements).
- Create a build directory (e.g. `mkdir geant4-v11.0.1-build`)
- Run makefiles generation with `cmake`:

  ```bash
  cd geant4-v11.0.1-build
  cmake -DCMAKE_INSTALL_PREFIX=../geant4-v11.0.1-install ../geant4-v11.0.1
  ```

- Build and install GEANT4 using generated makefiles. Parameter `-j2` starts the build in 2 parallel jobs,
  this number may be increased for a faster build.
  
  ```bash
  make -j2
  make install
  ```

- Specify a full path to your `geant4-v11.0.1-install` directory in `.env` file in `G4_CMAKE_PREFIX` variable.


### Building executables

After all the prerequisites are installed and `.env` is filled with corresponding values, just do

```bash
source scripts/build.sh
```

To cleanup the installation, use

```bash
source scripts/clean.sh
```

## TODO:

* Зафиксировать протокол входных и выходных данных Geant4-программы
* Наметить план по другим детекторам (Geant4-коды по аналогии с мюонным)
* Начальная версия snakemake-пайплайна для запуска CORSIKA/Geant4/промежуточных скриптов
* Общий скрипт сборки/установки для портативности
