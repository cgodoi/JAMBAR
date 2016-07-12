Template.Dispositivo.onCreated(function(){
this.editMode = new ReactiveVar(false);
});

Template.Dispositivo.helpers({
	updateDispositivoId: function(){
		return this._id;
	},
	editMode:function(){
		return Template.instance().editMode.get(); //instancia particular de template
	}
});

Template.Dispositivo.events({
	'click .toggle-menu':function(){
		Meteor.call('toggleDashboardDispositivo',this._id, this.inDashboard);
	},
	'click .fa-trash' : function(){
		Meteor.call('deleteDispositivo',this._id);
	},
	'click .fa-pencil' : function(event, template){
		template.editMode.set(!template.editMode.get());
	}
});