from firebase import firebase;
firebase = firebase.FirebaseApplication("https://tlmproyecto-e2269.firebaseio.com/",None);
result = firebase.get("datos",'');
valores = result.values();
num = len(result);
print(result);
contador = 1;
#Para recorrer los valores
for valor in valores:
    if(contador==num):
        print(valor.get('ejey'));
    contador = contador + 1;

