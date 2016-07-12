//Meteor.subscribe('recipes');
Template.DispositivoSingle.onCreated(function(){
	var self = this;
	self.autorun(function(){
		var id = FlowRouter.getParam('id');
		self.subscribe('SingleDispositivo',id);
	});
});

Template.DispositivoSingle.helpers({
	dispositivo:()=> {
		var id = FlowRouter.getParam('id');
		return Dispositivos.findOne({_id:id});
	}
});