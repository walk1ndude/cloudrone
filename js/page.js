var PAGE = { 
  pages : {},
  currentPage : '',
  
showPage : function(pagename) { 
    if (!!this.currentPage) {
      this.pages[this.currentPage].header.hide();
      this.pages[this.currentPage].content.hide();
    }
    
    this.pages[pagename].header.show();
    this.pages[pagename].content.show();
  
    this.currentPage = pagename;
  },
  
 setPages : function() {
      
    (function() {
      var header = ['Main', 'Main', 'Main', 'SelectDrone', 'FlyTask', 'Monitoring', 'Result'];
      var content = ['Main', 'About', 'Register', 'SelectDrone', 'FlyTask', 'Monitoring', 'Result'];

      for (var i in header) {
	PAGE.pages[content[i]] = {
	  'header' : $('#header' + header[i]),
	  'content' : $('#content' + content[i])
	}
      }
    }());
    
    var setVisibility = function(params) {
      params.domElement.on('click', function(event) {
	PAGE.showPage(params.page);
      
	var hide = params.domHide;
	if (hide) {
	  params.domHide.hide();
	}
	
	var map = params.map;
	if (map) {
	  CLOUDRONE.maps[map].invalidateSize(false);
	}
      
	var flags = params.flags;
	if (flags.showTable) {
	  WORKER_COMM.doShowDrones({
	    show : CLOUDRONE.SHOWPOLICY.SHOW_ALL
	  },
	  CLOUDRONE.templates.drone_show);
	}
      
	if (flags.showResults) {
	  WORKER_COMM.initResults();
	}
      });
    };
 
    setVisibility({
     domElement : $('#lMain'),
      page : 'Main'
    });
    
   setVisibility({
     domElement : $('#lAbout'),
      page : 'About'
    });
    
   setVisibility({
     domElement : $('#lRegister'),
      page : 'Register'
    });
    
   setVisibility({
     domElement : $('#lSelectDroneMain'),
      page : 'SelectDrone',
      domHide : $('#droneStateControl'),
      flags : {
	showTable : true
      }
    });
    
   setVisibility({
      domElement : $('#lSelectDroneFlyTask'),
      page : 'SelectDrone',
      domHide : $('#droneStateControl'),
     flags : {
	showTable : true
      }
    });
    
   setVisibility({
      domElement : $('#lFlyTask'),
      page : 'FlyTask',
      map : 'taskMap'
    });
    
  setVisibility({
     domElement : $('#lMonitoringFlyTask'),
      page : 'Monitoring',
      map : 'monitorMap'
    });
    
   setVisibility({
      domElement : $('#lResultFlyTask'),
      page : 'Result',
      map : 'resultMap',
     flags : {
	showResults : true
      }
    });
    
   setVisibility({
     domElement : $('#lSelectDroneMonitoring'),
      page : 'SelectDrone',
      domHide : $('#droneStateControl'),
      flags : {
	showTable : true
      }
    });
    
  setVisibility({
      domElement : $('#lFlyTaskMonitoring'),
      page : 'FlyTask'
    });
    
    setVisibility({
     domElement : $('#lMonitoring'),
      page : 'Monitoring',
      map : 'monitorMap'
    });
    
   setVisibility({
      domElement : $('#lResultMonitoring'),
      page : 'Result',
      map : 'resultMap',
     flags : {
	showResults : true
      }
    });

   setVisibility({
     domElement : $('#lSelectDroneResult'),
      page : 'SelectDrone',
      domHide : $('#droneStateControl'),
      flags : {
	showTable : true
      }
    });
    
   setVisibility({
      domElement : $('#lFlyTaskResult'),
      page : 'FlyTask'
    });
    
   setVisibility({
     domElement : $('#lMonitoringResult'),
      page : 'Monitoring',
      map : 'monitorMap'
    });
    
   setVisibility({
      domElement : $('#lResult'),
      page : 'Result',
      map : 'resultMap',
     flags : {
	showResults : true
      }
    });
   
    (function() {
      $('input:radio[name="showDrones"]').change(function() {
	if ($(this).val() == 'showDronesAll') {
	  WORKER_COMM.doShowDrones({
	    policy : CLOUDRONE.SHOWPOLICY.SHOW_ALL
	  },
	  CLOUDRONE.templates.drone_show);
	}
	else if ($(this).val() == 'showDronesUser') {
	  WORKER_COMM.doShowDrones({
	    policy : CLOUDRONE.SHOWPOLICY.SHOW_USER,
	    user : localStorage.id
	  },
	  CLOUDRONE.templates.drone_show);
	}
	else if ($(this).val() == 'showDronesFree') {
	  WORKER_COMM.doShowDrones({
	    policy : CLOUDRONE.SHOWPOLICY.SHOW_FREE,
	  },
	  CLOUDRONE.templates.drone_show);
	}
      });
    }());
    
    this.showPage('Main');
  },
  
  fetchRadioSelectDrones : function() {
    $('input:radio[name="showDrones"]').change(function() {
      if ($(this).val() == 'showDronesAll') {
	WORKER_COMM.doShowDrones({
	  show : CLOUDRONE.SHOWPARAMS.SHOW_ALL
	},
	CLOUDRONE.templates.drone_show);
      }
      else if ($(this).val() == 'showDronesUser') {
	WORKER_COMM.doShowDrones({
	  show : CLOUDRONE.SHOWPARAMS.SHOW_USER,
	  user : localStorage.id
	},
	CLOUDRONE.templates.drone_show);
      }
      else if ($(this).val() == 'showDronesFree') {
	WORKER_COMM.doShowDrones({
	  show : CLOUDRONE.SHOWPARAMS.SHOW_FREE,
	},
	CLOUDRONE.templates.drone_show);
      }
    })
  },
};

