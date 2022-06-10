make crackme &&
	strip crackme &&
	python target_append.py crackme 16384 &&
	cat crackme.py >> crackme
