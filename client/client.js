let outputContentElement = document.getElementById('outputContent');
let outputErrorElement = document.getElementById('outputError');

hideAllServices();
updateEndpoint();

function updateService() {
    hideAllServices();
    const servicesSelectElement = document.getElementById('service');
    const selectedService = servicesSelectElement.value;
    if (selectedService === 'none') {
        return;
    }

    let serviceElement = document.getElementById(selectedService);
    serviceElement.style.display = 'block';
}

function hideAllServices() {
    let servicesElement = document.getElementById('services');
    Array.from(servicesElement.children).forEach(service => {
        service.style.display = 'none';
    });
}

function getEndpointUrl() {
    const endpointElement = document.getElementById('endpoint');
    const endpoint = endpointElement.value;

    switch (endpoint) {
        case 'localDefault':
            return localServiceEndpointUrl;
        case 'production':
            return productionServiceEndpointUrl;
        default:
            return undefined;
    }
}

function updateEndpoint() {
    const endpointUrl = getEndpointUrl();

    let endpointUrlElement = document.getElementById('endpointUrl');
    endpointUrlElement.value = endpointUrl ? endpointUrl : '';
}

function makeApiRequest(url) {
    outputContentElement.innerHTML = '';
    outputContentElement.setAttribute('visibility', 'visible');
    outputErrorElement.setAttribute('visibility', 'hidden');
    fetch(url)
        .then(response => response.text())
        .then(text => {
            outputContentElement.setAttribute('visibility', 'hidden');
            outputContentElement.innerHTML = text;
        })
        .catch(error => {
            outputErrorElement.setAttribute('visibility', 'visible');
            outputErrorElement.innerHTML = `Error: ${error}`;
        });
}

function runBottles() {
    const bottleCountElement = document.getElementById('bottleCount');
    const bottleColourElement = document.getElementById('bottleColour');

    const bottleCount = bottleCountElement.value ? bottleCountElement.value : bottleCountDefault;
    const bottleColour = bottleColourElement.value ? bottleColourElement.value : bottleColourDefault;

    const endpointUrl = getEndpointUrl();
    makeApiRequest(`${endpointUrl}/btl?--bottles=${bottleCount}&--colour=${bottleColour}`);
}

function runFizzFuzzBop() {
    const fizzFuzzBopCountElement = document.getElementById('fizzFuzzBopCount');
    const fizzElement = document.getElementById('fizz');
    const fuzzElement = document.getElementById('fuzz');
    const bopElement = document.getElementById('bop');
    const isGameplayElement = document.getElementById('isGameplay');

    const count = fizzFuzzBopCountElement.value ? fizzFuzzBopCountElement.value : fizzFuzzBopCountDefault;
    const fizz = fizzElement.value ? fizzElement.value : fizzDefault;
    const fuzz = fuzzElement.value ? fuzzElement.value : fuzzDefault;
    const bop = bopElement.value ? bopElement.value : bopDefault;
    const isGameplay = isGameplayElement.checked;

    const baseEndpointUrl = getEndpointUrl();
    const gameplayQueryString = isGameplay ? '&--gameplay' : '';
    makeApiRequest(`${baseEndpointUrl}/fz?--count=${count}&--fizz=${fizz}&--fuzz=${fuzz}&--bop=${bop}${gameplayQueryString}`);
}

function runHello() {
    const endpointUrl = getEndpointUrl();
    makeApiRequest(`${endpointUrl}/hello`);
}

function runPotatoes() {
    const potatoCountElement = document.getElementById('potatoCount');
    const potatoOptionsElement = document.querySelector('input[name="potatoOptions"]:checked');
    
    const potatoCount = potatoCountElement.value ? potatoCountElement.value : potatoCountDefault;
    const potatoOption = potatoOptionsElement ? potatoOptionsElement.value : potatoOptionsDefault;

    const endpointUrl = getEndpointUrl();
    makeApiRequest(`${endpointUrl}/pt?--count=${potatoCount}&--${potatoOption}`);
}