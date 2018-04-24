all: files.txt condor.cfg src.tar.gz

files.txt: makelist.py
	./makelist.py

condor.cfg: makeconfig.py files.txt condor/tmpl.cfg
	./makeconfig.py

src.tar.gz: src/*
	tar -czvf src.tar.gz src/*

.PHONY: test

test: all
	rm -r test/*
	cp run.sh src.tar.gz test
	cd test; ./run.sh $(shell head -n3 files.txt)
