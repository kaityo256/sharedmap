# Merge std::map between processes

## Summary

Merge std::map between processes using serializable map.

## Usage

    $ make
    $ ./a.out

## Results

```
$ mpirun -np 4 ./a.out
before
rank = 0
key05:5
key08:8
key09:9

rank = 1
key06:6
key08:8

rank = 2
key08:8
key09:9
key10:10

rank = 3
key05:5
key07:7
key10:10

after
rank = 0
key05:5
key06:6
key07:7
key08:8
key09:9
key10:10

rank = 1
key05:5
key06:6
key07:7
key08:8
key09:9
key10:10

rank = 2
key05:5
key06:6
key07:7
key08:8
key09:9
key10:10

rank = 3
key05:5
key06:6
key07:7
key08:8
key09:9
key10:10
```

## LICENSE

* serializablemap.hpp:  BSL 1.0
* other files: MIT 
