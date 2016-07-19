//Meteor.subscribe('recipes');

Template.Dash.onCreated(function(){
	var self = this;
	self.autorun(function(){
		self.subscribe('dispositivos');
	});
});

Template.Dash.helpers({
	dispositivos:()=> {
		return Dispositivos.find({inDashboard:true},{sort: {sector:-1,tipoDispositivo: -1}});
	}
});