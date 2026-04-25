// index.h dosyası - KUBİİ Arayüzü
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html lang="tr"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body { background: #0d1117; color: white; font-family: sans-serif; margin: 0; overflow: hidden; display: flex; justify-content: center; align-items: center; height: 100vh; }
  .p { position: absolute; width: 100%; height: 100vh; transition: 0.5s ease; display: flex; flex-direction: column; align-items: center; justify-content: center; }
  #p1 { left: 0; } #p2 { left: 100%; }
  .eye { width: 120px; height: 120px; background: #bc02ff; border-radius: 50%; box-shadow: 0 0 50px #bc02ff; animation: pulse 2s infinite; }
  @keyframes pulse { 0%, 100% { transform: scale(1); } 50% { transform: scale(1.1); } }
  #chat-btn { position: fixed; bottom: 30px; right: 30px; background: #bc02ff; width: 65px; height: 65px; border-radius: 50%; display: flex; align-items: center; justify-content: center; cursor: pointer; font-size: 30px; box-shadow: 0 5px 15px rgba(188,2,255,0.5); z-index: 10; }
  .card { width: 90%; max-width: 400px; height: 80vh; background: #161b22; border: 2px solid #bc02ff; border-radius: 20px; display: flex; flex-direction: column; }
  #m { flex-grow: 1; overflow-y: auto; padding: 15px; display: flex; flex-direction: column; }
  .msg { padding: 12px; margin: 8px 0; border-radius: 12px; font-size: 14px; max-width: 85%; }
  .u { background: #bc02ff; align-self: flex-end; } .k { background: #30363d; align-self: flex-start; border-left: 3px solid #bc02ff; }
  .in { display: flex; padding: 15px; background: #0d1117; gap: 10px; border-bottom-left-radius: 20px; border-bottom-right-radius: 20px; }
  input { flex-grow: 1; background: #161b22; border: 1px solid #30363d; color: white; padding: 12px; border-radius: 8px; outline: none; }
  #back { margin-top: 15px; color: #bc02ff; cursor: pointer; font-weight: bold; }
</style></head><body>
<div id="p1" class="p">
  <div class="eye"></div><h2 style="color:#bc02ff; margin-top:30px;">KUBİİ DİNLİYOR...</h2>
  <div id="chat-btn" onclick="go(1)">💬</div>
</div>
<div id="p2" class="p">
  <div class="card">
    <div id="m"></div>
    <div class="in"><input type="text" id="in" placeholder="KUBİİ'ye yaz..."><button onclick="s()" style="background:#bc02ff; border:none; color:white; padding:10px 15px; border-radius:8px; cursor:pointer;">GİT</button></div>
  </div>
  <div id="back" onclick="go(0)">← GERİ DÖN</div>
</div>
<script>
  function go(p) { document.getElementById('p1').style.left = p ? '-100%' : '0'; document.getElementById('p2').style.left = p ? '0' : '100%'; }
  async function s() {
    let i=document.getElementById('in'); if(!i.value) return;
    let v=i.value; add(v,'u'); i.value='';
    let r=await fetch('/chat?msg='+encodeURIComponent(v));
    let t=await r.text(); add(t,'k');
  }
  function add(t,c) {
    let d=document.createElement('div'); d.className='msg '+c;
    d.innerText=(c=='u'?'Sen: ':'KUBİİ: ')+t;
    let box=document.getElementById('m'); box.appendChild(d); box.scrollTop=box.scrollHeight;
  }
</script></body></html>)rawliteral";