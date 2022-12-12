var tokenDevice;
var listDevice;

// create func handle
console.log('create func');

// logout
function handleLogout() {
    window.location.href = 'login.html';
}

// get D of Device
function getStateD1() {
    const getValueD1 = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D1");
    getValueD1.on('value',function(datasnapshot){
        let D1 = datasnapshot.val();	
        if (D1.state == "0" ){
        var isGenderMale = $('#a').prop('checked', D1.state);
        }
    else if (D1.state == "1" ){
        var isGenderMale = $('#a').prop('checked', D1.state);
        } 
    });
}

function getStateD2() {
    const getValueD2 = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D2");
    getValueD2.on('value',function(datasnapshot){
        let D2 = datasnapshot.val();	
        if (D2.state == "0" ){
        var isGenderMale = $('#b').prop('checked', D2.state);
        }
    else if (D2.state == "1" ){
        var isGenderMale = $('#b').prop('checked', D2.state);
        } 
    });
}

function getStateD3() {
    const getValueD3 = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D3");
    getValueD3.on('value',function(datasnapshot){
        let D3 = datasnapshot.val();	
        if (D3.state == "0" ){
        var isGenderMale = $('#c').prop('checked', D3.state);
        }
    else if (D3.state == "1" ){
        var isGenderMale = $('#c').prop('checked', D3.state);
        } 
    });
}

// ----------------------

// setState of Device
function setStateD1() {
    // controller D1    
    $('#a').click(function() {
        console.log('tokenDevice D1 = ' +tokenDevice);
        if($(this).is(':checked')) 
        {
            firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D1/state").set(true);
        } else 
        {
        firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D1/state").set(false);
        }
    });
}

function setStateD2() {
    $('#b').click(function() {
        if($(this).is(':checked')) 
        {
            firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D2/state").set(true);
        } else 
        {
        firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D2/state").set(false);
        }
    });
}

function setStateD3() {
    $('#c').click(function() {
        if($(this).is(':checked')) 
        {
            firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D3/state").set(true);
        } else 
        {
        firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D3/state").set(false);
        }
    });
}
//----------------------

// set TimeOn D1
function setTimeOnD1() {
    $('#timeOnD1').change(function() {
        console.log('change time on');
        let dateTime = $('#timeOnD1').val().replace("T", "/");
        console.log('dateTime = ' + dateTime);
        firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D1/time_on").set(dateTime);
    });
}

// getTimeOn D1
function getTimeOnD1() {
    const getValueD1 = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D1");
    getValueD1.on('value',function(datasnapshot){
        let D1 = datasnapshot.val();
        console.log('getTimeOnD1' + D1);
        $('#timeOnD1').val(D1.time_on.replace("/", "T"));
    });
}

// set TimeOn D2
function setTimeOnD2() {
    $('#timeOnD2').change(function() {
        console.log('change time on');
        let dateTime = $('#timeOnD2').val().replace("T", "/");
        console.log('dateTime = ' + dateTime);
        firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D2/time_on").set(dateTime);
    });
}

// getTimeOn D2
function getTimeOnD2() {
    const getValueD2 = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D2");
    getValueD2.on('value',function(datasnapshot){
        let D2 = datasnapshot.val();
        console.log('getTimeOnD2' + D2);
        $('#timeOnD2').val(D2.time_on.replace("/", "T"));
    });
}

// set TimeOn D3
function setTimeOnD3() {
    $('#timeOnD3').change(function() {
        console.log('change time on');
        let dateTime = $('#timeOnD3').val().replace("T", "/");
        console.log('dateTime = ' + dateTime);
        firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D3/time_on").set(dateTime);
    });
}

// getTimeOn D3
function getTimeOnD3() {
    const getValueD3 = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D3");
    getValueD3.on('value',function(datasnapshot){
        let D3 = datasnapshot.val();
        console.log('getTimeOnD3' + D3);
        $('#timeOnD3').val(D3.time_on.replace("/", "T"));
    });
}

// set TimeOff D1
function setTimeOffD1() {
    $('#timeOffD1').change(function() {
        console.log('change time off D1');
        let dateTime = $('#timeOffD1').val().replace("T", "/");
        console.log('dateTime = ' + dateTime);
        firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D1/time_off").set(dateTime);
    });
}

// getTimeOff D1
function getTimeOffD1() {
    const getValueD1 = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D1");
    getValueD1.on('value',function(datasnapshot){
        let D1 = datasnapshot.val();
        console.log('timeOffD1' + D1);
        $('#timeOffD1').val(D1.time_off.replace("/", "T"));
    });
}

// set TimeOff D2
function setTimeOffD2() {
    $('#timeOffD2').change(function() {
        console.log('change time off D2');
        let dateTime = $('#timeOffD2').val().replace("T", "/");
        console.log('dateTime = ' + dateTime);
        firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D2/time_off").set(dateTime);
    });
}

// getTimeOff D2
function getTimeOffD2() {
    const getValueD2 = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D2");
    getValueD2.on('value',function(datasnapshot){
        let D2 = datasnapshot.val();
        console.log('timeOffD2' + D2);
        $('#timeOffD2').val(D2.time_off.replace("/", "T"));
    });
}

// set TimeOff D3
function setTimeOffD3() {
    $('#timeOffD3').change(function() {
        console.log('change time off D3');
        let dateTime = $('#timeOffD3').val().replace("T", "/");
        console.log('dateTime = ' + dateTime);
        firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D3/time_off").set(dateTime);
    });
}

// getTimeOff D3
function getTimeOffD3() {
    const getValueD3 = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D3");
    getValueD3.on('value',function(datasnapshot){
        let D3 = datasnapshot.val();
        console.log('timeOffD3' + D3);
        $('#timeOffD3').val(D3.time_off.replace("/", "T"));
    });
}

// set Name D1
function setNameD1() {
    $('#nameD1').change(function() {
        console.log('change name D1');
        let nameD1 = $('#nameD1').val()
        console.log('nameD1 = ' + nameD1);
        firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D1/name").set(nameD1);
    });
}

// get Name D1
function getNameD1() {
    const getValueD1 = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D1");
    getValueD1.on('value',function(datasnapshot){
        let D1 = datasnapshot.val();
        console.log('name D1' + D1);
        $('#nameD1').val(D1.name);
    });
}

// set Name D2
function setNameD2() {
    $('#nameD2').change(function() {
        console.log('change name D2');
        let nameD2 = $('#nameD2').val()
        console.log('nameD2 = ' + nameD2);
        firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D2/name").set(nameD2);
    });
}

// get Name D2
function getNameD2() {
    const getValueD2 = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D2");
    getValueD2.on('value',function(datasnapshot){
        let D2 = datasnapshot.val();
        console.log('name D2' + D2);
        $('#nameD2').val(D2.name);
    });
}

// set Name D3
function setNameD3() {
    $('#nameD3').change(function() {
        console.log('change name D3');
        let nameD3 = $('#nameD3').val()
        console.log('nameD3 = ' + nameD3);
        firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D3/name").set(nameD3);
    });
}

// get Name D3
function getNameD3() {
    const getValueD3 = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/D3");
    getValueD3.on('value',function(datasnapshot){
        let D3 = datasnapshot.val();
        console.log('name D3' + D3);
        $('#nameD3').val(D3.name);
    });
}

function handleOnChangeInputDevive() {
    console.log('handle on change input device');
    let nameDevice = $("#".concat(tokenDevice)).val()
    console.log('name Device = ' + nameDevice);
    firebase.database().ref().child("iot-home/devices/"+tokenDevice+"/name").set(nameDevice);
}

// get Name Device
function getNameDevice() {
    const getValueDevice = firebaseRef = firebase.database().ref().child("iot-home/devices/"+tokenDevice);
    getValueDevice.on('value',function(datasnapshot){
        let device = datasnapshot.val();
        console.log('device name' + device.name);
        $('#'.concat(tokenDevice)).val(device.name);
    });
}


// handleOnClickDevice
function handleOnClickDevice(paramTokenDevice) {
    console.log('handleOnClickDevice = '+ paramTokenDevice);
    if (window.tokenDevice != paramTokenDevice) {
        $("#".concat(paramTokenDevice)).attr('style',  'color:red');
        $("#".concat(tokenDevice)).attr('style',  'color:black');
    }
    window.tokenDevice  = paramTokenDevice;
    console.log('tokenDevice =' + tokenDevice);
    getStateD1();
    getStateD2();
    getStateD3();

    getTimeOnD1();
    getTimeOnD2();
    getTimeOnD3();

    getTimeOffD1();
    getTimeOffD2();
    getTimeOffD3();

    getNameD1();
    getNameD2();
    getNameD3();

    getNameDevice();
};
//--------------------




console.log('----------------');



jQuery(document).ready(function(){

    // create variable
    console.log('create variable');
    
    console.log('---------------');

    // get date time
    var myVar=setInterval(function(){Clock()},1000);
	function Clock() {
		a=new Date();
		w=Array("Chủ Nhật","Thứ hai","Thứ ba","Thứ tư","Thứ năm","Thứ sáu","Thứ bảy");
		var a=w[a.getDay()],
		w=new Date,
		d=w.getDate();
		m=w.getMonth()+1;
		y=w.getFullYear();
		h=w.getHours();
		mi=w.getMinutes();
		se=w.getSeconds();
		if(10>d){d="0"+d}
		if(10>m){m="0"+m}
		if(10>h){h="0"+h}
		if(10>mi){mi="0"+mi}
		if(10>se){se="0"+se}
		document.getElementById("date").innerHTML="Hôm nay: "+a+", "+d+" / "+m+" / "+y+" - "+h+":"+mi+":"+se+"";
		}

    // get list device
    console.log('get list device');
    var getListDevice = firebaseRef = firebase.database().ref().child("/iot-home/devices/list_device");
    getListDevice.on('value',function(datasnapshot){
        console.log(datasnapshot.val());
        window.listDevice = datasnapshot.val().split(';');
        window.listDevice.pop();
        console.log(window.listDevice);
        window.tokenDevice = window.listDevice[0];
        // display list device
        let count = 0;

        var listDeviceHtml = '<ul>'

        window.listDevice.forEach(function(tokenDevice) {
            let nameDevice


            // get Name device
            console.log('api : ' + "/iot-home/devices/"+tokenDevice+"/name");
            var getNameDevice = firebaseRef = firebase.database().ref().child("/iot-home/devices/"+tokenDevice+"/name");
            getNameDevice.on('value', function(datasnapshot) {
                nameDevice = datasnapshot.val()
                console.log('nameDevice = ' + nameDevice);
                listDeviceHtml += '<li> '+
                ' <input class="active form-control" value="'+nameDevice+'" id="'
                + tokenDevice
                +'" onClick="handleOnClickDevice(\'' + tokenDevice + '\')"'
                + 'onChange="handleOnChangeInputDevive()">'
                + '</input> </li>';

                count ++;
                console.log('count = ' + count);
                console.log('window.listDevice.length = ' + window.listDevice.length);
                console.log('condition = ' + (count == window.listDevice.length));
                if (count == window.listDevice.length) {
                    listDeviceHtml += '</ul>';
                    console.log(listDeviceHtml);
                    document.getElementById("listDevice").innerHTML = listDeviceHtml;
                    
                    console.log(tokenDevice == window.tokenDevice);
                    
                    $("#".concat(window.tokenDevice)).attr('style',  'color:red');
                    $("#".concat(tokenDevice)).attr('style',  'color:black');

                    console.log('default window tokenDevice = '+ window.tokenDevice);
                    console.log('--------------');
                }               
                });
        });

        
        

    // get/set D1
    setStateD1();
    getStateD1();
    
    // get/set D2    
    setStateD2();
    getStateD2();

    // get/set D3  
    setStateD3();
    getStateD3();

    // setTimeOn 
    setTimeOnD1();
    setTimeOnD2();
    setTimeOnD3();

    // getTimeOn 
    getTimeOnD1();
    getTimeOnD2();
    getTimeOnD3();


    // setTimeOff
    setTimeOffD1();
    setTimeOffD2();
    setTimeOffD3();

    // getTimeOff 
    getTimeOffD1();
    getTimeOffD2();
    getTimeOffD3();


    // getNameDn
    getNameD1();
    getNameD2();
    getNameD3();

    // setNameDn
    setNameD1();
    setNameD2();
    setNameD3();

    // set name Device
    setNameDevice();

    });


})