Template.Sector.onCreated(function(){
this.editMode = new ReactiveVar(false);
});

Template.Sector.helpers({
	updateSectorId: function(){
		return this._id;
	},
	editMode:function(){
		return Template.instance().editMode.get(); //instancia particular de template
	}
});

Template.Sector.events({
	'click .fa-trash' : function(){
		Meteor.call('deleteSector',this._id);
	},
	'click .fa-pencil' : function(event, template){
		template.editMode.set(!template.editMode.get());
	}
});

Template.TipoDispositivo.onCreated(function(){
this.editMode = new ReactiveVar(false);
});

Template.TipoDispositivo.helpers({
	updateTipoDispositvoId: function(){
		return this._id;
	},
	editMode:function(){
		return Template.instance().editMode.get(); //instancia particular de template
	}
});

Template.TipoDispositivo.events({
	'click .fa-trash' : function(){
		Meteor.call('deleteTipoDispositivo',this._id);
	},
	'click .fa-pencil' : function(event, template){
		template.editMode.set(!template.editMode.get());
	}
});
/*modulos*/
Template.Modulo.onCreated(function(){
this.editMode = new ReactiveVar(false);
});

Template.Modulo.helpers({
	updateModuloId: function(){
		return this._id;
	},
	editMode:function(){
		return Template.instance().editMode.get(); //instancia particular de template
	}
});

Template.Modulo.events({
	'click .fa-trash' : function(){
		Meteor.call('deleteModulo',this._id);
	},
	'click .fa-pencil' : function(event, template){
		template.editMode.set(!template.editMode.get());
	}
});
