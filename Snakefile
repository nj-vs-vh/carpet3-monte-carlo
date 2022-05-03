envvars:
    "CARPET3_BIN_DIR"

rule all:
    default_target: True
    input:
        pass
    output:
        pass
    shell:
        echo {output}


rule muon_detector:
    input:
        
