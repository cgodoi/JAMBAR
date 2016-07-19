//Meteor.subscribe('dispositivos');

Template.Dispositivos.onCreated(function(){
	var self = this;
	self.autorun(function(){
		self.subscribe('dispositivos');
		self.subscribe('sectores');
		self.subscribe('tipoDispositivos');
		self.subscribe('modulos');
	});
});

Template.Dispositivos.helpers({
	dispositivos:()=> {
		return Dispositivos.find();
	}
});

Template.Dispositivos.events({
'click .new-recipe':()=>{
	Session.set('nuevoDispositivo', true);
}
});