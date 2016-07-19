Modulos = new Mongo.Collection('modulos');

Modulos.allow({
	insert:function(userId,doc){
		return !!userId; //quien puede insertar, solo si eres user id (logeado)
	},
	update:function(userId,doc){
		return !!userId;//permitir actualizar collection
	}
});


ModuloSchema =  new SimpleSchema({
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
	deleteModulo:function(id){
		Modulos.remove(id);
	}
});

Modulos.attachSchema(ModuloSchema);