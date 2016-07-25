Dispositivos = new Mongo.Collection('dispositivos');

Dispositivos.allow({
	insert:function(userId,doc){
		return !!userId; //quien puede insertar, solo si eres user id (logeado)
	},
	update:function(userId,doc){
		return !!userId;//permitir actualizar collection
	}
});


Caracteristica = new SimpleSchema({
	nombre:{
		type:String
	},
	valor:{
		type:String
	}
});

DispositivoSchema =  new SimpleSchema({
	codigo:{
		type:String,
		label:"Codigo"
	},
	nombre:{
		type:String,
		label:"Nombre"
	},
	modulo:{
		type: String,
    	label: 'Modulo',
      	autoform: {
      	firstOption: 'Seleccione modulo del dispositivo',
      		options: function () {
        		return Modulos.find().map( function ( c ) {
            													return {label: c.nombre,
            															value: c._id };
            																	 }
            													);
      							},
    				}		
    			},
	tipoDispositivo: {
      	type: String,
      	label: 'Tipo dispositivo',
      	autoform: {
      	firstOption: 'Seleccione tipo de dispositivo',
      		options: function () {
        		return TipoDispositivos.find().map( function ( c ) {
            													return {label: c.nombre,
            															value: c._id };
            																	 }
            													);
      							},
    				}
	},
	 sector: {
      	type: String,
      	label: 'Sector',
      	autoform: {
      	firstOption: 'Seleccione Sector',
      		options: function () {
        		return Sectores.find().map( function ( c ) {
            													return {label: c.nombre,
            															value: c._id };
            																	 }
            													);
      							},
    				}
	},
	caracteristicas:{
		type:[Caracteristica]
	},
	inDashboard:{
		type:Boolean,
		defaultValue:false,
		optional:true,
		autoform:{
			type:"hidden"
		}
	},
	estado:{
		type:Boolean,
		defaultValue:false,
		optional:true,
		autoform:{
			type:"hidden"
		}
	},
	author:{
		type:String,
		label:"Author",
		autoValue:function(){
			return this.userId
		},
		autoform:{
			type:"hidden"
		}
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
	toggleDashboardDispositivo:function(id, currentState){
		Dispositivos.update(id,{
			$set:{
				inDashboard: !currentState
			}
		});
	},
	deleteDispositivo:function(id){
		Dispositivos.remove(id);
	},
	togglePrendeApaga:function(id,currentState){
		console.log(this);
		Dispositivos.update(id,{
			$set:{
				estado:!currentState
			}
		});
	}
});

Dispositivos.attachSchema(DispositivoSchema);