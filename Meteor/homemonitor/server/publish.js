 Meteor.publish('recipes',function(){
	//return Recipes.find({author:this.userId}) ;//solo las recipes del autor
	return Recipes.find() ;//solo las recipes del autor
 });
 Meteor.publish('dispositivos',function(){
	//return Dispositivos.find({author:this.userId}) ;//solo las recipes del autor
	return Dispositivos.find() ;//solo las recipes del autor
 });

 Meteor.publish('SingleRecipe',function(id){
	//return Recipes.find({author:this.userId}) ;//solo las recipes del autor
	check(id, String);
	return Recipes.find({_id:id}) ;//solo las recipes del autor
 });

 Meteor.publish('SingleDispositivo',function(id){
	//return Recipes.find({author:this.userId}) ;//solo las recipes del autor
	check(id, String);
	return Dispositivos.find({_id:id}) ;//solo las recipes del autor
 });