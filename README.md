# Fun as a Service!

This repository contains a Fermyon Spin application with a selection of fun command-line programs built as WebAssembly modules configured to run as components via the WebAssembly Gateway Interface (WAGI).

## Technologies

* C, C++
* WebAssembly
* Fermyon Spin
* HTML, CSS, JavaScript (vanilla)

## Requirements

The following need to be installed to build and run the application:

* GNU Make
* WASI SDK
* Spin CLI
* Python (Optional: Used for serving the client from the `Makefile`)
* Wasmtime (Optional: Useful for local running of generated WebAssembly modules)

To deploy the application to Fermyon Cloud you will need a Fermyon account and be logged into the Spin CLI.

## Getting Started

To build and serve the application, run:

```sh
make
make serve-api
```

The application can be accessed at http://localhost:3000.

A basic HTML client, deliberately designed in a retro Web 1.0 style, is included **for development purposes** in the `client` directory and can be run using:

```sh
make serve-client
```

The client can be accessed at http://localhost:8000.

> **Please note that for the client to work you will need to temporarily disable cross-origin restrictions in your browser.**

The `Makefile` in the root of the repository has numerous targets to build, serve, deploy and clean the application including the client.  The main ones are listed in the table below.

|Target|Description|
|-|-|
|`all`, `build`|Builds the application.|
|`serve-api`|Runs the application locally.|
|`serve-client`|Runs the client locally.|
|`clean`|Cleans the build.|

## Included Components

The included components are listed in the table below.

|Component|Endpoint|Description|Language|Source|
|-|-|-|-|-|
|Bottles!|`/btl`|Prints the lyrics to *Ten Green Bottles*, with a specified number and colour!<br />*(Slightly Modified [Gist](https://gist.github.com/BHazel/07990d15f0e0eae67e44dd722cb33e00))*|C|`btl.c`|
|Fizz Fuzz Bop|`/fz`|An expanded version of *FizzBuzz*, as taught to me by a friend!<br />*(Slightly Modified [Gist](https://gist.github.com/BHazel/fb2ae1c24e66ee594bcd5b555b698b04))*|C|`fz.c`|
|Hello, World!|`/hello`|The universal programmer's greeting!|C++|`hello.cpp`|
|Potatoes!|`/pt`|Prints the lyrics to <em>One Potato, Two Potato</em> with a specified number of potatoes or something else!<br />*(Slightly Modified [Gist](https://gist.github.com/BHazel/d352d1baa9f434ad5a3e340aa32d49e9))*|C|`pt.c`|

### Endpoint Parameters

Parameters to the endpoints are the same as command-line arguments for the original programs, formatted as query strings.  As an example for the `/btl` endpoint to set the lyrics to be 20 blue bottles:

```sh
# Original
./btl --bottles 20 --colour blue

# Query String
/btl?--bottles=20&--colour=blue
```

Please see the Gists or the source code in the `src` directory for the command-line arguments for the programs.

### Modifications

Some of the programs, as indicated above, include some slightly modified versions of gists on my GitHub account.  The only modificaiton for each one is the addition of the following at the start of the program:

```cpp
// C
#ifndef NO_CGI
    printf("Content-Type: text/plain\n\n");
#endif

// C++
#ifndef NO_CGI
    std::println("Content-Type: text/plain\n");
#endif
```

Sending the content type at the top of the output is required by the WAGI specification.  At present all other code is the same as in the original gists.

### Selecting Components for a Build

By default all components in the `src` directory are built and included in the build.  To choose specific components the `WASM_COMPONENTS` list in the `Makefile` can be overridden when calling `make`.  For example, to only build `btl` and `pt` run:

```sh
make WASM_COMPONENTS="btl pt"
```

## Creating a New WAGI Component

This section uses a simple random number generator written in C as an example of creating a new component.

> **To write a component in a langauge other than C or C++ please see the *Using a New Language* section below.**

* Create a source file in the `src` directory, e.g. `random.c`.  **Make sure the first output is a `Content-Type` as shown above.**

```c
// src/random.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
#ifndef NO_CGI
    printf("Content-Type: text/plain\n\n");
#endif
    srand(time(NULL));
    int random = rand() % 100;
    printf("%d\n", random);

    return 0;
}
```

* Create a TOML configuration file in the `conf` directory with the same name as the component, e.g. `random.toml`.  The `id`, `component` and `route` fields can be any value, however **the built WebAssembly file must also have the same name as the component, e.g. `random.wasm`.**  For this reason, and to follow conventions elsewhere, they might as well all be the same.

```toml
# random.toml

[[trigger.http]]
id = "trigger-random"
component = "random"
route = "random/..."
executor = { type = "wagi" }

[component.random]
source = "random.wasm"


```

The two blank lines at the end of the file are not strictly necessary, however, the file is appended to the generated `spin.toml` file as part of the build and makes it easier to read.

* Add the component name to the `WASM_COMPONENTS` list in the `Makefile`:

```makefile
# Makefile

WASM_COMPONENTS=btl fz hello pt random
```

If all works as expected the new component should now be included in the build!

### Updating the Client

Each component has a basic UI and backing script for simple testing in the client.  These are vanilla HTML, CSS and JavaScript and can be as simple or complex as needed.  To follow existing conventions:

**In `index.html`:**

* Add an option to the `service` dropdown with its `value` attribute set to the name of the component:

```html
<!-- index.html -->

<select id="service" onchange="updateService()">
    <!-- ... -->
    <option value="random">Random Number!</option>
    <!-- ... -->
</select>
```

* Add a UI in a child `<div/>` to the `services` `<div />` to set any required values for testing the component:

```html
<!-- index.html -->

<div id="services">
    <!-- ... -->
    <div id="random">
        <h3>Random!</h3>
        <h4>Generates a random number between 1 and 100!</h4>
        <button onclick="runRandom()">Get Random!</button>
    </div>
    <!-- ... -->
</div>
```

**In `constants.js`:**

* Add any constants required for the component, e.g. default values.

**In `client.js`:**

* Add a function to call the endpoint:

```js
function runRandom() {
    const endpointUrl = getEndpointUrl();
    makeApiRequest(`${endpointUrl}/random`);
}
```