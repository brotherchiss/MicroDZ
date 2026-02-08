import { initializeApp } from
"https://www.gstatic.com/firebasejs/10.7.1/firebase-app.js";
import { getAuth } from
"https://www.gstatic.com/firebasejs/10.7.1/firebase-auth.js";
import { getFirestore } from
"https://www.gstatic.com/firebasejs/10.7.1/firebase-firestore.js";

const firebaseConfig = {
  apiKey: "AIzaSyDX6AEeU53BxG1Rwcdrwmm3vzSmM6iTT6E",
  authDomain: "client-login-app-6bdcd.firebaseapp.com",
  projectId: "client-login-app-6bdcd",
  storageBucket: "client-login-app-6bdcd.firebasestorage.app",
  messagingSenderId: "257593926119",
  appId: "1:257593926119:web:423310b90d1149e0785d42",
  measurementId: "G-1PZFSQ47BG"
};

const app = initializeApp(firebaseConfig);

export const auth = getAuth(app);
export const db = getFirestore(app);
console.log("Firebase init OK");
