Service
=====

Simple [RPC](http://en.wikipedia.org/wiki/Remote_procedure_call) backend written in C circa 2005.

Usage
-----

*Service* was created with *Linux* in mind.

It may run in other *POSIX* systems but code changes (specially in signal/socket handling) are probably needed.

```bash
# Compile
$ make

# Run the service daemon
$ ./service
```

The default port is **6666** *UDP*.
You can check some client samples on *index.php* and *client.php* files.
