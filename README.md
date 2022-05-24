# Carpet-3 Monte-Carlo

## Setup

The Monte-Carlo simulation requires external dependencies and a build step.
The configuration and parameters for all these steps is stored in [`.env`](https://www.dotenv.org/) file.
Make sure to create your own private (not tracked in git) copy of it with

```bash
cp .env.example .env
```

To activate your current environment (i.e. set all the necessary environment variables), from project root run

```bash
source scripts/activate.sh
source scripts/activate.sh  --verbose  # to print out all the variables
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
  cmake -DGEANT4_INSTALL_DATA=ON -DCMAKE_INSTALL_PREFIX=../geant4-v11.0.1-install ../geant4-v11.0.1
  ```

- Build and install GEANT4 using generated makefiles. Parameter `-j2` starts the build in 2 parallel jobs,
  this number may be increased for a faster build.
  
  ```bash
  make -j2
  make install
  ```

- Specify a full path to your `geant4-v11.0.1-install` directory in `.env` file in `G4_CMAKE_PREFIX` variable.


#### CORSIKA

TBD


#### COAST and pyCOAST

TBD


### Building executables

After all the prerequisites are installed and `.env` is filled with corresponding values, just do

```bash
bash scripts/build.sh
```

To cleanup the installation, use

```bash
bash scripts/clean.sh
```

### Pipeline setup

The Monte-Carlo pipeline consists of multiple C++ executables and Python programs. To orchetrate their execution in
the correct order we use [`snakemake`](https://snakemake.readthedocs.io/en/stable/index.html). The full setup of our snakemake pipeline is described here.

#### Install mamba

We use [`mamba`](https://github.com/mamba-org/mamba) package manager to install third party Python dependencies, including `snakemake` itself.

You can download it from [here](https://github.com/conda-forge/miniforge#mambaforge) or install on top of existing `conda` installation:

```bash
conda install -n base -c conda-forge mamba
```

#### Install snakemake

```bash
export ENV_NAME=carpet3  # or choose your own name
mamba create -c conda-forge -c bioconda -n $ENV_NAME snakemake
mamba activate $ENV_NAME
```

## Running Snakemake pipeline

First you will need to create a directory for run data, for example

```bash
mkdir runs
# all the simulated files will live inside this directory
# the internal directory structure is defined in the Snakefile and
# must not be manually changed
mkdir runs/example
```

Finally, run snakemake with

```bash
snakemake \
  --directory runs/example \
  --cores 3
```

See [full list](https://snakemake.readthedocs.io/en/stable/executing/cli.html#all-options) of Snakemake command line options for reference
