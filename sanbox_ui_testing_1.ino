
#include <WiFi.h>
#include <WebServer.h>

#include <DNSServer.h>

const byte DNS_PORT = 53;


/* Put your SSID & Password */
const char* ssid = "SanBox";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
//IPAddress local_ip(192,168,1,1);
//IPAddress gateway(192,168,1,1);
//IPAddress subnet(255,255,255,0);


//IPAddress apIP(8,8,8,8);
IPAddress apIP(192,168,1,1);

WebServer ws(80);
DNSServer dnsServer;
//WiFiServer server(80);

//WiFiClient client;

uint8_t tube_1 = 25;
bool tube_1_status = HIGH;

uint8_t tube_2 = 33;
bool tube_2_status = HIGH;

String RedirectHTML() {
  String redirect_html = "<html>\n";
  redirect_html += "<head>\n";
  redirect_html += "<title>HTML Meta Tag</title>\n";
  redirect_html += "<meta http-equiv = \"refresh\" content = \"2; url = http://192.168.1.1\" />\n";
  redirect_html += "</head>\n";
  redirect_html += "<body>\n";
  redirect_html += "<p>Redirecting to your SanBox control page!</p>\n";
  redirect_html += "</body>\n";
  redirect_html += "</html>\n";
  return redirect_html;
}

String Knob_test() {
  String knob_ui = "<form method=\"post\" action=\"\">\n";
  knob_ui += "<div>\n";

 knob_ui += "<label for=\"name\">French Hens</label>\n";
knob_ui += "<input type=\"text\" name=\"french-hens\" id=\"french-hens\" value=\"3\">\n";
  knob_ui += "</div>\n";
    knob_ui += "<div>\n";

knob_ui += "<label for=\"name\">Turtle Doves</label>\n";
knob_ui += "<input type=\"text\" name=\"turtle-doves\" id=\"turtle-doves\" value=\"2\">\n";
  knob_ui += "</div>\n";
      knob_ui += "<div>\n";

knob_ui += "<label for=\"name\">Partridges</label>\n";
knob_ui += "<input type=\"text\" name=\"partridge\" id=\"partridge\" value=\"1\">\n";
  knob_ui += "</div>\n";
knob_ui += "<input type=\"submit\" value=\"Submit\" id=\"submit\">\n";
knob_ui += "</form>\n";
return knob_ui;
}

String SendHTML(uint8_t upper_tube_stat,uint8_t lower_tube_stat) {

  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>SanBox Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  
  ptr += ".button-on {\n";
  ptr += "background-color:#44c767;\n";
  ptr += "position: relative;\n";
  ptr += "display:block;\n";
  ptr += "margin: 0 auto;\n";
  ptr += "border-radius:150px;\n";
  ptr += "border:0px solid #18ab29;\n";
 //ptr += "display:inline-block;\n";
  ptr += "cursor:pointer;\n";
  ptr += "color:#ffffff;\n";
  ptr += "font-family:Arial;\n";
  ptr += "font-size:17px;\n";
  ptr += "padding:10px 80px;\n";
  ptr += "text-decoration:none;\n";
  ptr += "text-shadow:0px 1px 0px #2f6627;\n";
  ptr += "}\n";

  
  ptr += ".button-on:active {\n";
  ptr += "background-color:#44c767;\n";
  ptr += "position:relative;\n";
  ptr += "}\n";

  ptr += ".button-off {\n";
  ptr += "background-color:#44c767;\n";
  ptr += "position: relative;\n";
  ptr += "display:block;\n";
  ptr += "margin: 0 auto;\n";
  ptr += "border-radius:150px;\n";
  ptr += "border:0px solid #18ab29;\n";
 //ptr += "display:inline-block;\n";
  ptr += "cursor:pointer;\n";
  ptr += "color:#ffffff;\n";
  ptr += "font-family:Arial;\n";
  ptr += "font-size:17px;\n";
  ptr += "padding:10px 80px;\n";
  ptr += "text-decoration:none;\n";
  ptr += "text-shadow:0px 1px 0px #2f6627;\n";
  ptr += "}\n";

  
  ptr += ".button-off:active {\n";
  ptr += "background-color:#44c767;\n";
  ptr += "position:relative;\n";
  ptr += "}\n";

  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>SanBox Control</h1>\n";

  if(upper_tube_stat) {
    ptr +="<a class=\"button button-off\" href=\"/tube1off\">UPPER TUBE OFF</a>\n";
  } else {
    ptr +=  "<img src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADIAAAAyCAYAAAAeP4ixAAAACXBIWXMAABcSAAAXEgFnn9JSAAAFX0lEQVRogeVaXWgjVRT+zs32oYmKsCyCIKgP21rEH3wQ9MGWqgSSUoQ1mfTFB2l9aB+KhUIRihT6UOwiBVl8aquLzU9FpJl5cR+2dcEtlDa7VOLfi8JSX0QplqWImePDzJ3cmcx0k3SmWfWDQCZzcu/55nznnHtvAvxHQJ12IAjXr299ePnyB5NEBGZGPB5HqVQK9Pe+I7Kzs/Pq3NzcNWYOtNF1vcHv+4pIOp12vJeRCEK5XBZE5BiIiH1rGioJCSJCX1+fr/3Q0JDpso3Ir5YgScgIEBHW1gq+tiMjmutayqzjEUmn00xkPU8iskjki762zIy1tTXfex0lkkqlXHIiIuTzeZD93gsiAisuExG2t7cfATpIJJVKsYyARJCcVKj2zIz5+fn3AOBcBD7eE345kc/n7bvWs3Vnvgm/Z25//yn43o0Y3hILIFD3vlBKMhEhmUyeA86YiExs9ZV3ElsAMFGtfgsrAqqM1Lxwx2piYmIAOENppdNp9ja4QqHkanq5XM55H1S5VBs1X84kIlJOapktFEqKhUAul3VXKmZXnhBRQw+ZnJx8vD5CxPDr2HnP087lsgDgio5XQtJGgpl/Hhwc/EVeR0rEj0SxuK5cmQ0OxuNxFAruMqxGgogwNjb2pGEYT6g2kS1R1D7BzK7EJmKYpuWgadYTu7u7G8vLq65x/CLhJQFERESNhCRhPWVLAESEbPZN130AKBQKYK67NDKieVfAP+m6ftFvztClJUus2uwKxXVlKhOalnHsZSTy+aJCwpIcMzsvIcSPQSSAkCOiLgAlkWJxHXYbBwCHhCo579JE0zLwPIwfdF3vPWnu0Ij4bYpKpc/BzDBRg0AMmcwlF4F4PI7l5VXFYYamuRN7fHz80WQy+eu95g9FWt5mx8xWJLhmfyAcEgAghEAikbATu74/0jQNaoFg5u+bIQGEEBG12ZmmCSGEq9kx15DN1ptdPflLrnHUvLHxna7r/ttDH8Racfru3b9evH27cqenp+f9paWl2uHh4ZZ6n4g8fQLIZjOujp1IJHD16mcuG58Se8swjGda8a2liAwNDTVISJWCzAkLJrLZbMMYxaK7OqnREkKgVqvdMgzj+Vb8aomIX5cG6tVpfX3d2r3Z194+IRNbEgBEg5wmJibOJ5PJ31vkAKDJZJ+env4j6B4Rob+/X7lmh4REIpHAyuqnik0MmpZxeoQdkb12SQBNEDk+Pr5YrVYfPslma2vLiUwmk3FJjoiUEmuCiJDJXLIJEYQQYOZdXddfaJcE0IS0giTVDOLxOFZWPgEApymqcrI79s1yufxSu3NInEhE9ge/E40gyBLsLrEmGAJZJRLMjFgs9s3GxsbLbXuvINBDb4VqFVYZrh8ayLyRJEZHRx8aHh7+s+0JPPDNkUqlcqNdEvJ7V658BEDYy46M9/6NMEkAARE5TV6oKBaLDb2EiL4ul8uvhDG+a1zvB3JDdFp4m6W9hLlmGMbrpx7cBw3SEiKcLYp3bcXMX0VFAvAh0t3dfTPsSZgZs7Ozb4U9ropIc8SZxJaX3y9NYaEhItVq9bWwJ5FLkSjhekKapv12dHR0PtQJrFzhcrkc6dGTM/j+/v5jYZOwcRg1CcA++93d3X1jZmbmi7AHjzInvIgBwMHBwQyAZ8PoHxJnSQKwpTU+Pv4lgFMlpLJsr5w1CUBJ9sXFxcrm5uZzpxmsEwQknMOHrq6uj5uVlr2PAFDv4J0kAdjS2tvbe7cZEsyMhYWFBw3DoN7ep1MAcOHChXc6TQJQpNXM/mNqaqprYGDg78i9agPOT29sHQuq51xHAB6QF97/fvzrsLq6+nanffhf4R+kKoBADQk5dwAAAABJRU5ErkJggg==\" width=\"50\" height=\"50\"/>\n";  
    ptr +="<a class=\"button button-on\" href=\"/tube1on\">UPPER TUBE ON</a>\n";
  }

  if(lower_tube_stat) {
    ptr +="<a class=\"button button-off\" href=\"/tube2off\">LOWER TUBE OFF</a>\n";
  } else {
    ptr +=  "<img src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADIAAAAyCAYAAAAeP4ixAAAACXBIWXMAABcSAAAXEgFnn9JSAAAFX0lEQVRogeVaXWgjVRT+zs32oYmKsCyCIKgP21rEH3wQ9MGWqgSSUoQ1mfTFB2l9aB+KhUIRihT6UOwiBVl8aquLzU9FpJl5cR+2dcEtlDa7VOLfi8JSX0QplqWImePDzJ3cmcx0k3SmWfWDQCZzcu/55nznnHtvAvxHQJ12IAjXr299ePnyB5NEBGZGPB5HqVQK9Pe+I7Kzs/Pq3NzcNWYOtNF1vcHv+4pIOp12vJeRCEK5XBZE5BiIiH1rGioJCSJCX1+fr/3Q0JDpso3Ir5YgScgIEBHW1gq+tiMjmutayqzjEUmn00xkPU8iskjki762zIy1tTXfex0lkkqlXHIiIuTzeZD93gsiAisuExG2t7cfATpIJJVKsYyARJCcVKj2zIz5+fn3AOBcBD7eE345kc/n7bvWs3Vnvgm/Z25//yn43o0Y3hILIFD3vlBKMhEhmUyeA86YiExs9ZV3ElsAMFGtfgsrAqqM1Lxwx2piYmIAOENppdNp9ja4QqHkanq5XM55H1S5VBs1X84kIlJOapktFEqKhUAul3VXKmZXnhBRQw+ZnJx8vD5CxPDr2HnP087lsgDgio5XQtJGgpl/Hhwc/EVeR0rEj0SxuK5cmQ0OxuNxFAruMqxGgogwNjb2pGEYT6g2kS1R1D7BzK7EJmKYpuWgadYTu7u7G8vLq65x/CLhJQFERESNhCRhPWVLAESEbPZN130AKBQKYK67NDKieVfAP+m6ftFvztClJUus2uwKxXVlKhOalnHsZSTy+aJCwpIcMzsvIcSPQSSAkCOiLgAlkWJxHXYbBwCHhCo579JE0zLwPIwfdF3vPWnu0Ij4bYpKpc/BzDBRg0AMmcwlF4F4PI7l5VXFYYamuRN7fHz80WQy+eu95g9FWt5mx8xWJLhmfyAcEgAghEAikbATu74/0jQNaoFg5u+bIQGEEBG12ZmmCSGEq9kx15DN1ptdPflLrnHUvLHxna7r/ttDH8Racfru3b9evH27cqenp+f9paWl2uHh4ZZ6n4g8fQLIZjOujp1IJHD16mcuG58Se8swjGda8a2liAwNDTVISJWCzAkLJrLZbMMYxaK7OqnREkKgVqvdMgzj+Vb8aomIX5cG6tVpfX3d2r3Z194+IRNbEgBEg5wmJibOJ5PJ31vkAKDJZJ+env4j6B4Rob+/X7lmh4REIpHAyuqnik0MmpZxeoQdkb12SQBNEDk+Pr5YrVYfPslma2vLiUwmk3FJjoiUEmuCiJDJXLIJEYQQYOZdXddfaJcE0IS0giTVDOLxOFZWPgEApymqcrI79s1yufxSu3NInEhE9ge/E40gyBLsLrEmGAJZJRLMjFgs9s3GxsbLbXuvINBDb4VqFVYZrh8ayLyRJEZHRx8aHh7+s+0JPPDNkUqlcqNdEvJ7V658BEDYy46M9/6NMEkAARE5TV6oKBaLDb2EiL4ul8uvhDG+a1zvB3JDdFp4m6W9hLlmGMbrpx7cBw3SEiKcLYp3bcXMX0VFAvAh0t3dfTPsSZgZs7Ozb4U9ropIc8SZxJaX3y9NYaEhItVq9bWwJ5FLkSjhekKapv12dHR0PtQJrFzhcrkc6dGTM/j+/v5jYZOwcRg1CcA++93d3X1jZmbmi7AHjzInvIgBwMHBwQyAZ8PoHxJnSQKwpTU+Pv4lgFMlpLJsr5w1CUBJ9sXFxcrm5uZzpxmsEwQknMOHrq6uj5uVlr2PAFDv4J0kAdjS2tvbe7cZEsyMhYWFBw3DoN7ep1MAcOHChXc6TQJQpNXM/mNqaqprYGDg78i9agPOT29sHQuq51xHAB6QF97/fvzrsLq6+nanffhf4R+kKoBADQk5dwAAAABJRU5ErkJggg==\" width=\"50\" height=\"50\"/>\n";
    ptr +="<a class=\"button button-on\" href=\"/tube2on\">LOWER TUBE ON</a>\n";
  }

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

String SendHTML1(uint8_t tube1stat,uint8_t tube2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>SanBox Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".SanBoxControl {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="</body>\n";
  ptr +="<h1>SanBox Control</h1>\n";
  //ptr +="<h3></h3>\n";
  
  if(tube1stat)
  {
    ptr +="<p>TUBE 1 Status: ON</p><a class=\"button button-off\" href=\"/tube1off\">OFF</a>\n";
  }
  else
  {
    ptr +=  "<img src=\"data:image/jpeg;base64,/9j/4AAQSkZJRgABAQEAlgCWAAD/2wBDAAgGBgcGBQgHBwcJCQgKDBQNDAsLDBkSEw8UHRofHh0aHBwgJC4nICIsIxwcKDcpLDAxNDQ0Hyc5PTgyPC4zNDL/2wBDAQkJCQwLDBgNDRgyIRwhMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjL/wAARCAAyADIDASIAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwD3+iiigApksscELzTSLHFGpZ3c4VQOSST0FEssdvC800iRxRqWd3YBVUckknoK8j1nWNR+JmsHRNELw6JAwa4uGBAkGeGYenHyp3xk4x8oBbvfF+v+NNVuNK8JEWthGv7y+cFWIwRnOCUBP3QBu4zxyBEmpeLPh7Jb3GuXo1XSJ5jHIBKZJEJGQQXAOcKSBkrwehINaPhjX9Jtddh8K+HrFp7GJXae/DZy4HLnA+YEgLuyBkgDgDNDx7fjxRfW3hTR42ubuO5WS4mQEpAeUw2AeBv+Y9unJyAAer0VFbQJa2sNvHnZEiou45OAMDNFAEtVrrULKxZFu7y3ty4ZkEsqpuCjLEZPOByfSodZ1i00HSp9SvmZYIRztGWYk4AA9SSB/PA5r5v8Ra7deI9an1G6LDzDiOMtkRJ/Co+n0GTk96APQdY1jUviXrDaLohaDQ4WBnuCCN/PDMPTj5U6kjJxj5al7eC/KeCPBCf6Hz9rvM/67+8zMP4PVv4uFUYwGxNM17UNT0Sx8HaRBBZNcSMtxceZtM+T3PYY4I5LAADj5T0csq6Io8G+DR9o1mfK3t+CEbcoJIDE8EDPf5egyxJABHcXKeH418HeDg1xq9w228vUGG3Y5AP8OOe+EGf4skd/4Q8F2PhK1cRt9ovJeJblk2kjPCgZOB04zyfwwvg3whbeFNLWLEc18/M1wEGecfKpxnaMDg9+eM10tABRRRQB5T8Z7q3a302zF44uUcym1CttZGBAcnpkFSB3+Y15FXV/ElETx9qaoJOsZYu27JManj0HPT/9VRaF4C13xFpcuoWUMSwJkR+c+wzEZyE4wcEYySBnvwcAHORSyQTJNDI0cqMGR0OCpHIII6GvcvhToyWXhr7fNpzW95cscTSHLSxcFSB/Cp9O+M9CKZ4M+GVtobw6jqrLc6kjFkRTmKI9iMjLMOTk8AngZANeg0AFFFFABRRRQAUgAAAAwB0AoooAWiiigAooooAKKKKAP//Z\" width=\"50\" height=\"50\"/>";
    ptr += "\n";
    ptr +="<p>TUBE 1 Status: OFF</p><a class=\"button button-on\" href=\"/tube1on\">ON</a>\n";
  }

  if(tube2stat)
  {
    ptr +="<p>TUBE 2 Status: ON</p><a class=\"button button-off\" href=\"/tube2off\">OFF</a>\n";
  }
  else
  {
    ptr +=  "<img src=\"data:image/jpeg;base64,/9j/4AAQSkZJRgABAQEAlgCWAAD/2wBDAAgGBgcGBQgHBwcJCQgKDBQNDAsLDBkSEw8UHRofHh0aHBwgJC4nICIsIxwcKDcpLDAxNDQ0Hyc5PTgyPC4zNDL/2wBDAQkJCQwLDBgNDRgyIRwhMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjL/wAARCAAyADIDASIAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwD3+iiigApksscELzTSLHFGpZ3c4VQOSST0FEssdvC800iRxRqWd3YBVUckknoK8j1nWNR+JmsHRNELw6JAwa4uGBAkGeGYenHyp3xk4x8oBbvfF+v+NNVuNK8JEWthGv7y+cFWIwRnOCUBP3QBu4zxyBEmpeLPh7Jb3GuXo1XSJ5jHIBKZJEJGQQXAOcKSBkrwehINaPhjX9Jtddh8K+HrFp7GJXae/DZy4HLnA+YEgLuyBkgDgDNDx7fjxRfW3hTR42ubuO5WS4mQEpAeUw2AeBv+Y9unJyAAer0VFbQJa2sNvHnZEiou45OAMDNFAEtVrrULKxZFu7y3ty4ZkEsqpuCjLEZPOByfSodZ1i00HSp9SvmZYIRztGWYk4AA9SSB/PA5r5v8Ra7deI9an1G6LDzDiOMtkRJ/Co+n0GTk96APQdY1jUviXrDaLohaDQ4WBnuCCN/PDMPTj5U6kjJxj5al7eC/KeCPBCf6Hz9rvM/67+8zMP4PVv4uFUYwGxNM17UNT0Sx8HaRBBZNcSMtxceZtM+T3PYY4I5LAADj5T0csq6Io8G+DR9o1mfK3t+CEbcoJIDE8EDPf5egyxJABHcXKeH418HeDg1xq9w228vUGG3Y5AP8OOe+EGf4skd/4Q8F2PhK1cRt9ovJeJblk2kjPCgZOB04zyfwwvg3whbeFNLWLEc18/M1wEGecfKpxnaMDg9+eM10tABRRRQB5T8Z7q3a302zF44uUcym1CttZGBAcnpkFSB3+Y15FXV/ElETx9qaoJOsZYu27JManj0HPT/9VRaF4C13xFpcuoWUMSwJkR+c+wzEZyE4wcEYySBnvwcAHORSyQTJNDI0cqMGR0OCpHIII6GvcvhToyWXhr7fNpzW95cscTSHLSxcFSB/Cp9O+M9CKZ4M+GVtobw6jqrLc6kjFkRTmKI9iMjLMOTk8AngZANeg0AFFFFABRRRQAUgAAAAwB0AoooAWiiigAooooAKKKKAP//Z\" width=\"50\" height=\"50\"";
    ptr += "\n";
    ptr +="<p>TUBE 2 Status: OFF</p><a class=\"button button-on\" href=\"/tube2on\">ON</a>\n";
  }

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

void handle_OnConnect() {
  tube_1_status = HIGH;
  tube_2_status = HIGH;
  Serial.println("tube_1_status: OFF |tube_2_status: OFF");
  //ws.send(200, "text/html",RedirectHTML());

 // client.println(SendHTML(tube_1_status,tube_2_status));
 ws.send(200, "text/html", Knob_test());
//ws.send(200, "text/html",  SendHTML(HIGH,HIGH));
//  ws.send(200, "text/html", SendHTML(tube_1_status,tube_2_status)); 
}


void handle_tube_1_on() {
  tube_1_status = HIGH;
  Serial.println("tube_1_status: ON");
  //client.println(SendHTML(true,tube_2_status));
  ws.send(200, "text/html", SendHTML(true,tube_2_status)); 
}

void handle_tube_1_off() {
  tube_1_status = LOW;
  Serial.println("tube_1_status: OFF");
  //client.println(SendHTML(false,tube_2_status));
  ws.send(200, "text/html", SendHTML(false,tube_2_status)); 
}

void handle_tube_2_on() {
  tube_2_status = HIGH;
  Serial.println("tube_2_status: ON");
  //client.println(SendHTML(tube_1_status,true));
  ws.send(200, "text/html", SendHTML(tube_1_status,true)); 
}

void handle_tube_2_off() {
  tube_2_status = LOW;
  Serial.println("tube_2_status: OFF");
  //client.println(SendHTML(tube_1_status,false));
  ws.send(200, "text/html", SendHTML(tube_1_status,false)); 
}

void handle_NotFound(){
  //client.println("Not found");
  //ws.send(404, "text/plain", "Not 8 found");
  Serial.println("IN REDIRECT");
  ws.send(200, "text/html",RedirectHTML());
}


void setup() {
  pinMode(tube_1, OUTPUT);
  pinMode(tube_2, OUTPUT);
  //WiFiManager wm;
  
  Serial.begin(115200);
  //pinMode(tube_1, OUTPUT);
  //pinMode(tube_2, OUTPUT);

  WiFi.mode(WIFI_AP);

  
  //WiFi.mode(WIFI_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

 // WiFi.softAPConfig(local_ip, gateway, subnet);
  //delay(1000);
  WiFi.softAP("SanBox");
  //delay(1000);
  //wm.setConfigPortalTimeout(100000);
  
 // wm.startConfigPortal("SanBox");

 // dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);


  ws.on(F("/"), handle_OnConnect);
  ws.on(F("/tube1on"), handle_tube_1_on);
  ws.on(F("/tube1off"), handle_tube_1_off);
  ws.on(F("/tube2on"), handle_tube_2_on);
  ws.on(F("/tube2off"), handle_tube_2_off);
  ws.onNotFound(handle_NotFound);

  //server.begin();
  ws.begin();
  
  Serial.println("HTTP server started");
}
void loop() {
  
  //client = server.available();
  
    dnsServer.processNextRequest();
    ws.handleClient();
   
      if(tube_1_status)
      {digitalWrite(tube_1, LOW);}
      else
      {digitalWrite(tube_1, HIGH);}
  
      if(tube_2_status)
      {digitalWrite(tube_2, LOW);}
      else
      {digitalWrite(tube_2, HIGH);}
 
  /*if(client) {
    String currentLine = "";
    while (client.connected()) {
      ws->send(200, "text/html",SendHTML(HIGH, HIGH));
      break;
    }
    while (client.connected()) {
     // Serial.println("IN WHILE 2");
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {        
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }*/
}

String sanbox_testing_ui_1 () {


String ui_test = "<html>"; 

ui_test += "<head>";
ui_test +=  "<script src=\"shared/jquery.js\" type=\"text/javascript\"></script>";
ui_test +=  "<script src=\"shared/shiny.js\" type=\"text/javascript\"></script>";
ui_test +=  "<link rel=\"stylesheet\" type=\"text/css\" href=\"shared/shiny.css\"/>";
ui_test +="</head>";

ui_test +="<body>";

ui_test +=  "<h1>HTML UI</h1>";

ui_test +=  "<p>";
 ui_test +=   "<label>Distribution type:</label><br />";
  ui_test +=  "<select name=\"dist\">";
   ui_test +=  " <option value=\"norm\">Normal</option>";
   ui_test +=   "<option value=\"unif\">Uniform</option>";
  ui_test +=  "<option value=\"lnorm\">Log-normal</option>";
   ui_test +=   "<option value=\"exp\">Exponential</option>";
  ui_test +=  "</select>";
 ui_test += "</p>";

ui_test +=  "<p>";

ui_test +=    "<label>Number of observations:</label><br />";
  ui_test +=  "<input type=\"number\" name=\"n\" value=\"500\" min=\"1\" max=\"1000\" />";

ui_test +=  "</p>";

ui_test +=  "<h3>Summary of data:</h3>";
ui_test += "<img src=\"data:image/jpeg;base64,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\" width=\"100\" height=\"100\" />";
ui_test +=  "<pre id=\"summary\" class=\"shiny-text-output\"></pre>";

 ui_test += "<h3>Plot of data:</h3>";
ui_test +=  "<div id=\"plot\" class=\"shiny-plot-output\"";
      ui_test += "style=\"width: 100%; height: 300px\"></div>";

 ui_test += "<h3>Head of data:</h3>";
ui_test +=  "<div id=\"table\" class=\"shiny-html-output\"></div>";

ui_test +="</body>";
ui_test +="</html>";

return ui_test;
}
