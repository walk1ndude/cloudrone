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

  CLOUDRONE.map = L.map('taskMap').setView([0, 0], 0);
  
  $("window").bind("unload", function(eventObject) {
      alert('here');
      return true;
    });
  
  $('#bFlightTaskInput').change(function(eventObject) {
    CLOUDRONE.loadFlightTask(eventObject);
  });
})
