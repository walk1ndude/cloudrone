function markerInit(markers) {
    /*
    var markerListListener = new ROSLIB.Topic({
        ros : ros,
        name : '/cloudrone/marker',
        messageType : 'cloudrone/Markers'
    });
    
    markerListListener.subscribe(function(marker) {
        printMarkerInfo(markers, marker);
    });
    */
    CLOUDRONE.getMarkerListClient = new ROSLIB.Service({
        ros : CLOUDRONE.ros,
        name : '/cloudrone/getMarker',
        serviceType : 'cloudrone/GetMarker'
    });
    
    var req = [
        new ROSLIB.ServiceRequest({}),
        new ROSLIB.ServiceRequest({})
    ];
    
    for (var number in req) {
        getMarkerListClient.callService(req[number], function(marker) {
	  printMarkerInfo(markers, marker);
        });
    }
}

function markerSelected(id) {

  if (CLOUDRONE.markers[selectedMarker]) {
    $('#markerId'+ CLOUDRONE.selectedMarker).css("background-color", "#FFFFFF");
    CLOUDRONE.markers[CLOUDRONE.selectedMarker].marker.closePopup();
    CLOUDRONE.markers[CLOUDRONE.selectedMarker].marker.togglePopup();
   }
   
   $('#markerId'+id).css("background-color", "#DDDDDD");
   CLOUDRONE.selectedMarker=id;
   $('#delMarker').show();
   CLOUDRONE.markers[CLOUDRONE.selectedMarker].marker.bindPopup("Выбран").openPopup();
}

function addMarker(e) {
  m = L.marker(e.latlng).addTo(maps['taskMap']);
    CLOUDRONE.markers[m['_leaflet_id']]={};
    CLOUDRONE.markers[m['_leaflet_id']].marker=m;
    CLOUDRONE.markers[m['_leaflet_id']].latlng=m['_latlng'];
    CLOUDRONE.markers[m['_leaflet_id']].checked='checked';
    CLOUDRONE.markers[m['_leaflet_id']].photo='../images/drones/no-available-image.png';
    CLOUDRONE.markers[m['_leaflet_id']].type='Тип';
    CLOUDRONE.markers[m['_leaflet_id']].info='Дополнительная информация';
/*	$('#markerstable').append('<tr id="marker_id'+m['_leaflet_id']+'" onClick="markerselected('+m['_leaflet_id']+');"><td><input type="checkbox" '+markers[m['_leaflet_id']].checked+'></input></td><td><img src="'+markers[m['_leaflet_id']].photo+'" width="50px"></img></td><td>'+markers[m['_leaflet_id']].type+'</td><td>'+markers[m['_leaflet_id']].info+'</td></tr>');*/
}

function onClickDelMarker() {
  CLOUDRONE.maps['taskMap'].removeLayer(CLOUDRONE.markers[CLOUDRONE.selectedMarker].marker);
  $('#markerId'+CLOUDRONE.selectedMarker).remove();
  delete(CLOUDRONE.markers[CLOUDRONE.selectedMarker]);
}


function printMarkerInfo(markers, marker) {
  $(markers).append("<table><tr><td><input type='checkbox' name='marker'></input></td><td><img width=20px height=20px src='../images/"
    + marker.type + ".png'></img></td><td>"
    + marker.name + '</td><td align="right">'
    + marker.type + '</td>'
    + '</table>');
}