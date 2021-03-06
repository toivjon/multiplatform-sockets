# multiplatform-sockets
[![Build Status](https://img.shields.io/travis/toivjon/multiplatform-sockets/master.svg)](https://travis-ci.org/toivjon/multiplatform-sockets)
[![Coverage Status](https://coveralls.io/repos/github/toivjon/multiplatform-sockets/badge.svg?branch=master)](https://coveralls.io/github/toivjon/multiplatform-sockets?branch=master)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)

A simple multiplatform header-only C++ library wrapper for UDP and TCP sockets.

## Features

This library contains following features:

- A support for TCP and UDP.
- A support for IPv4 and IPv6.
- TODO

## Compilation

TODO

## Notes
There are some interesting additional things that should be noted here.

- UWP applications acting as server cannot be targeted with loopback addresses. This is
  because of the network isolation, which prevents local non-UWP applications to target
  UWP applications as they would be in runnin in the same machine. In other words, this
  means that non-UWP clients cannot use loopback to connect UWP servers. However, the
  vice versa seems to work i.e. UWP clients can connect to non-UWP servers with loopback.

- UWP applications require permissions "Internet (Client & Server)", "Internet (Client)"
  and/or "Private Networks (Client & Server)" based on how the applications wants to
  handle network operations. See Microsoft's documentation about these permissions.
