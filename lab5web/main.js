port = "8082"
last_press = "Sec"

async function sec_btn() {
    const { data } = await axios.get("http://127.0.0.1:" + port + "/sec");

    fill_table("Second", parse_data(data));
    last_press = "Sec"
}

async function hour_btn() {
    const { data } = await axios.get("http://127.0.0.1:" + port + "/hour");

    fill_table("Hour", parse_data(data));
    last_press = "Hour"
}

async function day_btn() {
    const { data } = await axios.get("http://127.0.0.1:" + port + "/day");

    fill_table("Day", parse_data(data));
    last_press = "Day"
}

function parse_data(data) {
    if (data.trim() == "") {
        return []
    }
    d = data.split(' ');
    dt = [];
    for (let i = 0; i < d.length; i += 2) {
        dt.push([d[i], d[i + 1]]);
    }
    console.log(dt)
    return dt;
}

function fill_table(period, data) {
    setTimeout(() => {
        if (last_press == "Sec") {
            sec_btn();
        } else if (last_press == "Hour") {
            hour_btn();
        } else if (last_press == "Day") {
            day_btn();
        }
    }, 1000);
    document.getElementById("period_str").innerHTML = "Period: " + period;
    if (data.length == 0) {
        document.getElementById("table").innerHTML = "";
        return;
    };
    
    let str = ""
    for (let i = 0; i < data.length; i++) {
        str += `<tr><td>${timeConverter(data[i][0])}</td><td>${data[i][1]}</td></tr>`
    }
    document.getElementById("table").innerHTML = str;
}

function timeConverter(UNIX_timestamp) {
    var a = new Date(UNIX_timestamp * 1000);
    var months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];
    var year = a.getFullYear();
    var month = months[a.getMonth()];
    var date = a.getDate();
    var hour = a.getHours();
    var min = a.getMinutes();
    var sec = a.getSeconds();
    var time = date + ' ' + month + ' ' + year + ' ' + hour + ':' + min + ':' + sec;
    return time;
}

window.onload = sec_btn;