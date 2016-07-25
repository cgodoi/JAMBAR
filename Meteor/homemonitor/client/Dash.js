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
	}
});


Template.Dash.events({
	'click .toggle-btn':function(event, template){
		Meteor.call('togglePrendeApaga',this._id, this.estado);
	}
});