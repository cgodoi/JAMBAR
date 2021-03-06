//Meteor.subscribe('dispositivos');

Template.Parametros.onCreated(function(){
	var self = this;
	self.autorun(function(){
		self.subscribe('sectores');
		self.subscribe('tipoDispositivos');
		self.subscribe('modulos');
	});
});

Template.Parametros.helpers({
	sectores:()=> {
		return Sectores.find();
	},
	tipoDispositivos:()=> {
		return TipoDispositivos.find();
	},
	modulos:()=> {
		return Modulos.find();
	}
});

Template.Parametros.events({
'click #btnNuevoSector':()=>{
	Session.set('nuevoSector', true);
},
'click #btnNuevoTipoDispositivo':()=>{
	Session.set('nuevoTipoDispositivo', true);
}
,
'click #btnNuevoModulo':()=>{
	Session.set('nuevoModulo', true);
}

});