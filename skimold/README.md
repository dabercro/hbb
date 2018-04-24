# Skimming over Hbb ntuples

- Modify `makelist.py` to point to the directory and site that you want to search for n-tuples.
- Modify `makeconfig.py` to give the version and number of jobs that you want.
- Optionally modify `run.sh` to point to a CrombieTools installation of your own or to use a different skimming script.

To generate the condor submission files type:

```
make
```

To check that this should work over condor type:

```
make test
```

Submit:

```
condor_submit condor.cfg
```

That's it!
