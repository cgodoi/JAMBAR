TipoDispositivos = new Mongo.Collection('tipoDispositivos');

TipoDispositivos.allow({
	insert:function(userId,doc){
		return !!userId; //quien puede insertar, solo si eres user id (logeado)
	},
	update:function(userId,doc){
		return !!userId;//permitir actualizar collection
	}
});


TipoDispositivoSchema =  new SimpleSchema({
	nombre:{
		type:String,
		label:"Nombre"
	},
	codigo:{
		type:String,
		label:"Codigo"
	},
	createdAt:{
		type:Date,
		label:"Created At",
		autoValue: function(){
			return new Date()
		},
		autoform:{
			type:"hidden"
		}
	}
});

Meteor.methods({
	deleteTipoDispositivo:function(id){
		TipoDispositivos.remove(id);
	}
});

TipoDispositivos.attachSchema(TipoDispositivoSchema);