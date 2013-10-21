$(document).ready(function() {
 
  Number.prototype.mod = function(n) { return ((this%n)+n)%n; }
  
  PAGE.setPages();
    
  WORKER_COMM.initRos({
    roshostname : 'localhost',
    rosport : 8080,
    mjpeghostname : 'localhost',
    mjpegport : 8081
  });
  
  WORKER_COMM.doSign({
      user : {
	id : localStorage.id
      }
  }, CLOUDRONE.templates.sign);

  CLOUDRONE.fetchMaps();
  
 /* 
  $("window").unload(
    CLOUDRONE.doKillNodes({
      user : localStorage.id
    })
  );
 */ 
  document.getElementById('flightTaskInput').addEventListener('change', CLOUDRONE.loadFlightTask, false);
})
