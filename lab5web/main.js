port = "8083"

function sec_btn() {
    fetch("http://127.0.0.1:" + port + "/sec", {
        headers: {
            'Accept': '*/*',
            'Access-Control-Allow-Origin': '*',
            'Access-Control-Allow-Encoding': 'gzip, deflate, br',
            'Access-Control-Allow-Language': 'ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7',
            'Sec-Fetch-Dest': 'script',
            'Sec-Fetch-Mode': 'no-cors',
            'Sec-Fetch-Site': 'cross-site',
        }
    }).then(function (response) {
        return response.json();
    }).then(function (data) {
        console.log(data);
    });
}

function hour_btn() {
    fetch("http://127.0.0.1:" + port + "/hour").then(function (response) {
        return response.json();
    }).then(function (data) {
        console.log(data);
    });
}

function day_btn() {
    fetch("http://127.0.0.1:" + port + "/day").then(function (response) {
        return response.json();
    }).then(function (data) {
        console.log(data);
    });
}

function parse_data() {

}

function fill_table(data) {

}