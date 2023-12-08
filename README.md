# templatec
A simple C template

The main branch is for normal C programs with no other dependency than [custom errno](https://github.com/nukingdragons/custom-errno.git)
The cpwn branch extends from this template and also depends on [cpwn](https://github.com/nukingdragons/cpwn.git), and is used primarily for developing exploits or proof of concepts.

# Dependencies
Depends on [custom errno](https://github.com/nukingdragons/custom-errno.git)

The order of the include directories matter, refer to the [custom errno](https://github.com/nukingdragons/custom-errno.git) repository for more information.

The provided Makefile takes care of this, but if you wish to add more libraries, make sure to pay attention to the include order.
