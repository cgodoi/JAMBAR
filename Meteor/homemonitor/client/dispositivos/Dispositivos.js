//Meteor.subscribe('dispositivos');

Template.Dispositivos.onCreated(function(){
	var self = this;
	self.autorun(function(){
		self.subscribe('dispositivos');
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