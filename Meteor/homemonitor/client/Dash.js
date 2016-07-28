//Meteor.subscribe('recipes');

Template.Dash.onCreated(function(){
	var self = this;
	self.autorun(function(){
		self.subscribe('dispositivos');
		self.subscribe('tipoDispositivos');
		self.subscribe('sectores');
	});
});

Template.Dash.helpers({
	dispositivos:function(pSector,pTipoDispositivo){
		return Dispositivos.find({inDashboard:true,sector:pSector, tipoDispositivo:pTipoDispositivo},{sort: {sector:-1,tipoDispositivo: -1}});
	},
	tipoDispositivos:()=>{
		return TipoDispositivos.find();
	},
	sectores:()=>{
		return Sectores.find();
	},
	compare:function(v1, v2) {
  		if (typeof v1 === 'object' && typeof v2 === 'object') {
    		return _.isEqual(v1, v2); // do a object comparison
  		} else {
    		return v1 === v2;
	    }
	}    
});


Template.Dash.events({
	'click .toggle-btn':function(event, template){
		Meteor.call('togglePrendeApaga',this._id, this.estado);
	}
});