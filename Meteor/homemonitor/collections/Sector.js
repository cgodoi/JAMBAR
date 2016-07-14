Sectores = new Mongo.Collection('sectores');

Sectores.allow({
	insert:function(userId,doc){
		return !!userId; //quien puede insertar, solo si eres user id (logeado)
	},
	update:function(userId,doc){
		return !!userId;//permitir actualizar collection
	}
});


SectorSchema =  new SimpleSchema({
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
	deleteSector:function(id){
		Sectores.remove(id);
	}
});

Sectores.attachSchema(SectorSchema);