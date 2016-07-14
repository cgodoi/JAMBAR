Template.NuevoSector.events({
	'click .fa-close' : function(){
		Session.set('nuevoSector',false)
	}
});

Template.NuevoTipoDispositivo.events({
	'click .fa-close' : function(){
		Session.set('nuevoTipoDispositivo',false)
	}
});