# BranchPredictorSimulator

Steps to run this simulator:

  For Windows:
  
    - Rename "Makefile.windows" to "Makefile"
    - In command window, type:
      - make [This will create all the object files and the executable sim file]
      - sim <command>
      - make clean [This will delete all the object files and also the executable sim file]
  
  For Linux or Unix:
  
    - Rename "Makefile.unix" to "Makefile"
    - In command window, type:
      - make [This will create all the object files and the executable sim file]
      - ./sim <command>
      - make clean [This will delete all the object files and also the executable sim file]
 
 Commands:
 
    For 'bimodal' branch predictor
    ./sim bimodal <M2> <trace_file>
    
    For 'gshare' branch predictor
    ./sim gshare <M1> <N> <trace_file>
    
    For 'hybrid' branch predictor
    ./sim hybrid <K> <M1> <N> <M2> <trace_file>
    
    Where:
    M1 - number of PC bits used to index the gshare table
    M2 - number of PC bits used to index the bimodal table
    N - number of global branch history register bits used to index the gshare table
    K - number of PC bits used to index the chooser table
