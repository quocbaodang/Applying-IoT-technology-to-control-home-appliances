var account;

function redirect(username, password) {
    if (username == window.account.username && password == window.account.password ){
        console.log('login thanh cong');
        $('#formLogin').attr('action', 'index.html').submit();
    } else {
        alert('Login Fail')
    }
}

function login() {
    const username = $("#username").val()
    const password = $("#password").val()

    redirect(username, password)
    
}


jQuery(document).ready(function(){
    const getAccount = firebaseRef = firebase.database().ref().child("iot-home/account");
    getAccount.on('value',function(datasnapshot){
        window.account = datasnapshot.val();
    });
})