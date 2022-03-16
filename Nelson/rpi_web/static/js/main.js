function send() {
    let msg = document.getElementById('msg').value;
    if (msg == '') return;
    document.getElementById('msg').value = '';
    fetch(`/add/${msg}`);
}

function updateMsgs(msgs) {
    let ul = document.createElement('ul');
    for (msg of msgs) {
        let li = document.createElement('li');
        li.innerHTML = msg;
        ul.appendChild(li);
    }
    let div = document.getElementById('msgs');
    div.innerHTML = "";
    div.appendChild(ul);
}

function getMsgs() {
    fetch('/msgs')
        .then(res => res.json())
        .then(msgs => updateMsgs(msgs))
        .catch(err => {throw err});
    setTimeout(getMsgs,1000);
}

document
    .getElementById("send")
    .addEventListener(
        "click",
        event => {
            event.preventDefault();
            send();
        });

getMsgs();
