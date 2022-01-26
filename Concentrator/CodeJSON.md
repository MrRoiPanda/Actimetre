## json file


```json
{
    "N° d'étude" : 6821,
    "Technicien": "NAR",
    "Animal" : "Rat ou Souris",
    "Groupes": {
        "1": [{
            "members": {"1": "MAC", "2": "MAC", "3": "MAC"},
            "duree du test" : 86400,
            "heure de début": "8 AM",
            "durée du jour" : 14400,
            "Intensité lumineuse" : "lux ou lumen ou candela ?" 
        }],
        "2" :[{
            "members": {"4": "MAC", "5": "MAC", "6": "MAC"},
            "duree du test" : 86400,
            "heure de début": "8 AM",
            "durée du jour" : 14400,
            "Intensité lumineuse" : "lux ou lumen ou candela ?" 
        }]
    }
}
```

```arduino
#include <Arduino.h>
#include <ArduinoJson.h>

DynamicJsonDocument doc(1024);

void setup() {
  Serial.begin(115200);

}

void loop() {
  doc["Netude"] = 6821;
  doc["Technicien"] = "NAR";
  doc["Animal"] = "Rat ou Souris";

  JsonObject Groupes = doc.createNestedObject("Groupes");

  JsonObject Groupes_1_0 = Groupes["1"].createNestedObject();

  JsonObject Groupes_1_0_members = Groupes_1_0.createNestedObject("members");
  Groupes_1_0_members["1"] = "MAC";
  Groupes_1_0_members["2"] = "MAC";
  Groupes_1_0_members["3"] = "MAC";
  Groupes_1_0["duree_du_test"] = 86400;
  Groupes_1_0["heure_de_début"] = "8 AM";
  Groupes_1_0["durée_du_jour"] = 14400;
  Groupes_1_0["Intensité_lumineuse"] = "lux ou lumen ou candela ?";

  JsonObject Groupes_2_0 = Groupes["2"].createNestedObject();

  JsonObject Groupes_2_0_members = Groupes_2_0.createNestedObject("members");
  Groupes_2_0_members["4"] = "MAC";
  Groupes_2_0_members["5"] = "MAC";
  Groupes_2_0_members["6"] = "MAC";
  Groupes_2_0["duree_du_test"] = 86400;
  Groupes_2_0["heure_de_début"] = "8 AM";
  Groupes_2_0["durée_du_jour"] = 14400;
  Groupes_2_0["Intensité_lumineuse"] = "lux ou lumen ou candela ?";

  serializeJsonPretty(doc, Serial);
  while (1)
  {
    
  }
}
```
