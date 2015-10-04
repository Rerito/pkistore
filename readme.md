
# PKI Store open source implementation #

The objective behind this project is to design a set of classes to interact
with and represent a PKI. The main idea is to use appropriate data structures to
model a full certificate arborescence.

# Public API #

The public API offers a set of instructions to build a PKI and process
authentication requests with it.

Currently I came up with the following routines:

  - Add/Remove certificate
  - Add/Remove CRL
  - Certificate authentication

I also wish to let the user attach a payload to a certificate (maybe using clever
templating). At this point, this is just an idea.
