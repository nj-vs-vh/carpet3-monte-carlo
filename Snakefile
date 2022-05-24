envvars:
    "CARPET3_BIN_DIR",


detectors = ["muon1", "muon2"]
datnames = ["DAT000001", "DAT000002"]
throw_indices = [1, 2, 3]


rule all:
    default_target: True
    input:
        expand(
            "detector_responses/{datname}.{throw_idx}.{detector}.response",
            datname=datnames,
            throw_idx=throw_indices,
            detector=detectors,
        ),
    shell:
        "echo {input}"


rule fake_particle_stream:
    output:
        "particle_streams/{datname}.{throw_idx}.muon{muon_det_idx}.prtcls",
    log:
        "particle_streams/{datname}.{throw_idx}.muon{muon_det_idx}.log",
    params:
        n_muons=100,
    script:
        "test/generate_particle_stream.py"


rule muon_detector:
    input:
        "particle_streams/{datname}.{throw_idx}.muon{muon_det_idx}.prtcls",
    params:
        bin_dir=os.environ["CARPET3_BIN_DIR"],
    output:
        "detector_responses/{datname}.{throw_idx}.muon{muon_det_idx}.response",
    log:
        "detector_responses/{datname}.{throw_idx}.muon{muon_det_idx}.log",
    shell:
        "{params.bin_dir}/MuonDetector {input} {output} &> {log}"
