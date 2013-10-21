var CLOUDRONE = {
  
 KOSTILEV : 360.0/20.0,
  
 selectedDrone : 0,
  pickedDrone : 0,
  
  selectedMarker : 0,
  markerPopup : null,
  
  counter : 0,
  interval : null,
  
  markers : {},
  
  drones : {},
  
  STATES : {
   'Free' : 0,
    'Selected' : 1,
    'OnTask' : 2,
    'WaitTask' : 3,
    'WaitNavdata' : 4 // special status (not present in db), for sending commands only on first navdata arrival
  },
  
  WRITESTATES : {
    'WaitTask' : 'ожидание полетного задания',
    'WaitLaunch' : 'ожидание начала полетного задания',
    'OnTask' : 'выполнение полетного задания',
    'OnComplete' : 'завершение полетного задания',
  },

  SHOWPOLICY : {
   'SHOW_ALL' : 0,
    'SHOW_USER' : 1,
    'SHOW_FREE' : 2
  },
    
  templates : {
    sign : {
      success : {
	id : 'sign_success',
	domElements : [
	  {
	    element : '#sign',
	    method : 'hide'
	  },
	  {
	    element : '#lSelectDroneMain',
	    method : 'show'
	  }
	]
      },
      failure : {
	id : 'sign_failure',
	domElements : [
	  {
	    element : '#signState',
	    method : 'html',
	    params : ['Ошибка! Вход не выполнен.']
	  }
	]
      }
    },
    
    reg : {
      success : {
	id : 'reg_success',
	domElements : [
	  {
	    element : '#register',
	    method : 'hide'
	  },
	  {
	    element : '#lSelectDroneMain',
	    method : 'show'
	  }
	]
      },
      failure : {
	id : 'reg_failure',
	domElements : [
	  {
	    element : '#registerState',
	    method : 'html',
	    params : ['Ошибка! Регистрация не выполнена']
	  }
	]
      }
    },
    
    drone_show : {
      success : {
	id : 'drone_show_success',
      },
      failure : {
	id : 'drone_show_failure',
	alerts : ['Ошибка! Невозможно показать БИТС']
      }
    },
    
    drone_pick : {
      success : {
	id : 'drone_pick_success',
	maps : ['taskMap'],
	pages : ['FlyTask'],
	domElements : [
	  {
	    element : '#droneStateControl',
	    method : 'show'
	  }
	]
      },
      failure : {
	id : 'drone_pick_failure',
	alerts : ['Невозможно изменить статус БИТС']
      }
    },
    
   drone_start : {
      success : {
	id : 'drone_start_success',
      },
      failure : {
	id : 'drone_pick_failure',
	alerts : ['Невозможно изменить статус БИТС']
      }
    },
    
   init_nodes : {
      success : {
	id : 'init_nodes_success',
      },
      failure : {
	id : 'init_nodes_failure',
	alerts : ['Невозможно выполнить запуск новых нодов']
      }
   },
    
    kill_nodes : {
      success : {
	id : 'kill_nodes_success',
      },
      failure : {
	id : 'kill_nodes_failure',
	alerts : ['Невозможно выгрузить ноды']
      }
    },
    
    get_info : {
      success : {
	id : 'get_info_success',
      },
      failure : {
	id : 'get_info_failure',
	alerts : ['Невозможно получить необходимую информацию']
      }
   }
  },
  
  maps : {
    'taskMap' : {},
    'monitorMap' : {},
    'resultMap' : {}
  },
 
  showDrones : function(response) {
    var drones = response.drones;
  
    $('#droneTable').html('<tr><th>Фото</th><th>Название</th><th>Модель</th><th>Расположение</th><th>Статус</th><th colspan="2"></th></tr>');
  
    for (var i = 0; i < drones.length; i ++ ) {
      
      drone = drones[i];
      
      id = drone.id;
      drone.model = eval(drones[i].model);
      
      var isOwned = drone.user == localStorage.id;
      var isFree = drone.state == CLOUDRONE.STATES['Free'];
      var isOnTask = drone.state == CLOUDRONE.STATES['OnTask'];
     
      $('#droneTable').append('<tr id="droneId' + id
	+ '" onClick="CLOUDRONE.selectDrone(' + id + ');"><td align="center"><img src="'
	+ drone.model.photo +'" width="50px"></img></td><td align="center">'
	+ drone.name + '</td><td align="center">'
	+ drone.model.name +'</td><td align="center">'
	+ drone.location +'</td><td align="center">'
	+ (isFree ? 'Свободен' : 'Занят') + '</td><td align="center"><button onClick="CLOUDRONE.pickDrone('
	+ id + ');" ' + ((isOwned || isFree) ? '' : 'disabled')
	+ '>Выбрать</button></td><td align="center"><button onClick="CLOUDRONE.stopTask('
	+ id + ');" ' + ((isOwned && isOnTask) ? '' : 'disabled')
	+ '>Остановить</button></td></tr>');
    }
      
    if (response.refresh) {
      CLOUDRONE.drones = response.drones;
   }    
  },

  selectDrone : function(id) {
    $('#droneId' + CLOUDRONE.selectedDrone).css("background-color", "#FFFFFF");
    $('#droneId' + id).css("background-color", "#DDDDDD");
  
    var specs = '';
    var specsArray = CLOUDRONE.drones[id].model.specs;
      
    for (spec in specsArray) {
      specs += '<tr><td><b>' + spec + '</b></td><td>' + specsArray[spec] + '</td></tr>'
    }

    $('#selectDroneInfo').html('<h3>Информация о выделенном БПЛА</h3><table><tr><td><img src="'
    + CLOUDRONE.drones[id].model.photo+'" width="100px" style="float:left"></img></td><td><b>Модель:</b> '
    + CLOUDRONE.drones[id].model.name+'</td><tr><td colspan="2"><b>Характеристики</b></td></tr>'
    + specs + '</table>');
  
    CLOUDRONE.selectedDrone = id;
  },
  
  unselectDrone : function() {
    $('#selectDroneInfo').empty();
  },
  
  pickDrone : function(id) {
      /**/
    $('#markers').empty();
       
    if(CLOUDRONE.drones[id].name=='TestDroneObj') {
      var info = '<table>';
      info+='<tr><td>' + "<img src='object/m1.png'/>" + '</td><td>' + 'Эталон класса 1'+ '</td><td>' + 'Растр'+ '</td></tr>';
      info+='<tr><td>' + "<img src='object/m2.png'/>" + '</td><td>' + 'Эталон класса 2'+ '</td><td>' + 'Вектор'+ '</td></tr>';
      info+='</table>';
      
      $('#markers').html(info);
    }
    /**/
   
    this.pickedDrone = id;
    
    WORKER_COMM.doSetState({
	id : id,
	pstate : CLOUDRONE.drones[id].state,
	nstate : CLOUDRONE.STATES['Selected']
      },
      CLOUDRONE.templates.drone_pick);
    
  },
  
  setState : function(id, sstate) {
    CLOUDRONE.drones[id].state = sstate;
  },
  
  getState : function(id) {
    return CLOUDRONE.drones[id].state;
  },
 
  doSign : function() {
    return WORKER_COMM.doSign({
	user : {
	  id : $('#signId').val(),
	  password : $('#signPassword').val()
	}
     },
      this.templates.sign);
  },
  
  doRegister : function() {
    return WORKER_COMM.doRegister({
      user : {
	  id : $('#registerId').val(),
	  password : $('#registerPassword').val()
	}
     },
      this.templates.reg);
  },

  fetchMaps : function() { 
    for (map in this.maps) {
      this.maps[map] = L.map(map).setView([0, 0], 0);
      L.tileLayer('../tiles/{z}/{y}/{x}.png', {maxZoom: 2,}).addTo(this.maps[map]);
     this.maps[map].markers = [];
    }
  },

  onClickStart : function() {
    var pickedDrone = CLOUDRONE.pickedDrone;
    var drone = CLOUDRONE.drones[pickedDrone];
    var state = CLOUDRONE.getState(pickedDrone);
    
    function onSelected() {
      $('#markersInfo').empty();
      
      $('#Stop').attr('disabled', '');
      $('#bStart').attr('disabled', 'disabled');
      
      drone.taskTime = new Date().getTime();
      
      $('#elapsedTime').html('0:0');
      $('#droneState').html(CLOUDRONE.WRITESTATES['OnTask']);
	
      PAGE.showPage('Monitoring');
      
      WORKER_COMM.doSetState({
	id : pickedDrone,
	pstate : state,
	nstate : CLOUDRONE.STATES['OnTask']
      },
      CLOUDRONE.templates.drone_start);
      
      setInterval(function() {
	CLOUDRONE.timerClick()
      },1000); 
    };
    
    switch (state) {
      case CLOUDRONE.STATES['Selected'] :
	onSelected();
	break;
    };
  },
  
  onClickStop : function() {
    var pickedDrone = CLOUDRONE.pickedDrone;
    var drone = CLOUDRONE.drones[pickedDrone];
    var state = CLOUDRONE.getState(pickedDrone);
    
    function onTask() {
      $('#bStop').prop('disabled', true);
      $('#bStart').prop('disabled', false);
      
      WORKER_COMM.killNodes({
	drone : drone
      });
      
      $('#droneState').html(CLOUDRONE.WRITESTATES['OnComplete']);
      PAGE.showPage('Result');
    }
    
    switch (state) {
      
      case CLOUDRONE.STATES['OnTask'] :
	onTask();
	break;
    };
  },
  
  timerClick : function() {
    var pickedDrone = CLOUDRONE.pickedDrone;
    var drone = CLOUDRONE.drones[pickedDrone];
    
    var currentTime = new Date().getTime() - drone.taskTime;
    var minutes = Math.floor(currentTime / 60000);
    var seconds = Math.floor((currentTime / 1000) % 60);
    $('#elapsedTime').html(((minutes < 10) ? '0' : '') + minutes + ':' + ((seconds < 10) ? '0' : '') + seconds);
   
    /*
    if(CLOUDRONE.drones[CLOUDRONE.pickedDrone].name=='TestDroneObj')
    {
    
      if(this.counter == 10)
      {
	var info = '<tr><td><div id="videoMarker">Обнаруженные маркеры:</td></tr>';
      info += '<tr><td>' + "<img src='object/1.png'/>" + '</td><td>' + 'Класс1'+ '</td></tr>';
      $('#markersInfo').append(info);
      }
      
      
      if(this.counter == 12)
      {
      var info = '<tr><td>' + "<img src='object/2.png'/>" + '</td><td>' + 'Класс2'+ '</td></tr>';
      $('#markersInfo').append(info);
      }
    }
    */
 },
  
  emptyNavdataInfo : function() {
    $('#sensorsInfo').empty();
  },
  
  printNavdataInfo : function(data, value) {
   $('#sensorsInfo').append('<tr><td>' + data + '</td><td>' + Math.round(value * 1000) / 1000  + '</td></tr>'); 
  },
  
  printMarkers : function(marker) {
    //$('#markersInfo').empty();
    
    var info = '<tr><td><div id="videoMarker">Обнаруженные маркеры:</td></tr>';
    /*for(field in marker) {
      if (marker.hasOwnProperty(field) && field != 'view') {
	info += '<tr><td>' + field + '</td><td>' + marker[field] + '</td></tr>';
      }
    }*/
    info += '<tr><td>' + "<img src='object/"+marker["viewid"]+".png'/>" + '</td><td>' + (marker["classid"] == 1 ? 'Класс1':'Класс2')+ '</td></tr>';
   // $('#markersInfo').append(info);
  },
  
  loadFlightTask : function(evt) {
    var patt = new RegExp(/^[ \t]*goto([ \t]*(\-)?(\d)*(\.)?(\d)*){4}[ \t]*$/);
    
    function sendCommands(flightPlan) {

     for(i in CLOUDRONE.maps.taskMap._layers) {
         if(CLOUDRONE.maps.taskMap._layers[i]._path != undefined) {
	    CLOUDRONE.maps.taskMap.removeLayer(CLOUDRONE.maps.taskMap._layers[i]);
	 }
      }
	
      CLOUDRONE.drones[CLOUDRONE.pickedDrone].cmds = ["c start"];
	  
      if (flightPlan.length > 0) {
	  var flightCmds = flightPlan.split('\n');
	  var markerId = 0;
	  
	  for (var c = 0; c < flightCmds.length; c++) {
	    if (!!flightCmds[c]) {
	         
	      if (flightCmds[c].match(patt)) { // if goto numbers
		var numbers = flightCmds[c].trim().split(" ");
		
		for (var j = 1; j <= 2; j++) {
		  numbers[j] *= CLOUDRONE.KOSTILEV;
		  while (numbers[j] < 0) numbers[j] += 360.;
		  while (numbers[j] > 360) numbers[j] -= 360.;
		}
		
		CLOUDRONE.maps.taskMap.markers[markerId ++] = L.marker([numbers[1], numbers[2]]).addTo(CLOUDRONE.maps.taskMap);
		//CLOUDRONE.maps.taskMap.markers[markerId ++] = L.marker([numbers[3]*CLOUDRONE.KOSTILEV, //numbers[4]*CLOUDRONE.KOSTILEV]).addTo(CLOUDRONE.maps.taskMap);
	      }
	      
	      CLOUDRONE.drones[CLOUDRONE.pickedDrone].cmds.push("c " + flightCmds[c]);
	    }
	  }
	}
    }
    
    function drawPolyline() {
      var map = CLOUDRONE.maps.taskMap
      var markers = [];
      
      for (var i = 0; i < map.markers.length; i++) {
	markers.push(map.markers[i].getLatLng());
      }
      
      L.polyline(markers,{color: 'red'}).addTo(map);
    }
    
    var files = evt.target.files;
    for (var i = 0, f; f = files[i]; i++) {

      var reader = new FileReader();
      
      reader.onload = function(e) {
	  var xml = e.target.result;
	  sendCommands($(xml).find("flightPlan").text()); 
	  drawPolyline();  
	  var objectList = $(xml).find("objectList").text();
      };
      reader.readAsBinaryString(f);
    }
    
    $('#bStart').removeAttr('disabled');
    $('#droneState').html(CLOUDRONE.WRITESTATES['WaitLaunch']);
    
  },
  
  //very old and untested code, maybe del ?
  
  addMarker : function(e) {
    m = L.marker(e.latlng).addTo(maps['taskMap']);
      this.markers[m['_leaflet_id']]={};
      this.markers[m['_leaflet_id']].marker=m;
      this.markers[m['_leaflet_id']].latlng=m['_latlng'];
      this.markers[m['_leaflet_id']].checked='checked';
      this.markers[m['_leaflet_id']].photo='../images/drones/no-available-image.png';
      this.markers[m['_leaflet_id']].type='Тип';
      this.markers[m['_leaflet_id']].info='Дополнительная информация';
/*	$('#markerstable').append('<tr id="marker_id'+m['_leaflet_id']+'" onClick="markerselected('+m['_leaflet_id']+');"><td><input type="checkbox" '+markers[m['_leaflet_id']].checked+'></input></td><td><img src="'+markers[m['_leaflet_id']].photo+'" width="50px"></img></td><td>'+markers[m['_leaflet_id']].type+'</td><td>'+markers[m['_leaflet_id']].info+'</td></tr>');*/
   },
   
   markerSelected : function markerSelected(id) {
      if (this.markers[this.selectedMarker]) {
	$('#markerId'+ this.selectedMarker).css("background-color", "#FFFFFF");
	CLOUDRONE.markers[CLOUDRONE.selectedMarker].marker.closePopup();
    CLOUDRONE.markers[CLOUDRONE.selectedMarker].marker.togglePopup();
   }
   
   $('#markerId'+id).css("background-color", "#DDDDDD");
   CLOUDRONE.selectedMarker=id;
   $('#delMarker').show();
   CLOUDRONE.markers[CLOUDRONE.selectedMarker].marker.bindPopup("Выбран").openPopup();
}
}  
