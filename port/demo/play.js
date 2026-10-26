"use strict";

const scenes = window.BYZANTINE_SCENES;
const canvas = document.getElementById("game");
const context = canvas.getContext("2d");
const message = document.getElementById("message");
const page = document.getElementById("page");
const previous = document.getElementById("previous");
const next = document.getElementById("next");
const ordered = [4, 5, 6, 7];
const images = new Map();
let current = 4;
let hovered = null;

function areaFor(direction) {
  const index = ordered.indexOf(current);
  const target = ordered[index + direction];
  return scenes[current].areas.find(area => area.target === target);
}

function draw() {
  context.fillStyle = "#000";
  context.fillRect(0, 0, 640, 420);
  const image = images.get(current);
  if (image?.complete && image.naturalWidth) {
    context.drawImage(image, 0, 20, 640, 400);
  }
  if (hovered) {
    context.fillStyle = "rgba(222, 178, 91, .18)";
    context.strokeStyle = "rgba(235, 202, 131, .8)";
    context.lineWidth = 2;
    context.fillRect(hovered.x, hovered.y, hovered.width, hovered.height);
    context.strokeRect(hovered.x, hovered.y, hovered.width, hovered.height);
  }
  const index = ordered.indexOf(current);
  page.textContent = `${index + 1} / ${ordered.length}`;
  previous.disabled = !areaFor(-1);
  next.disabled = !areaFor(1);
}

function goTo(target) {
  if (!scenes[target]) return;
  current = target;
  hovered = null;
  message.textContent = "";
  draw();
}

function point(event) {
  const rect = canvas.getBoundingClientRect();
  return {x: (event.clientX - rect.left) * 640 / rect.width,
          y: (event.clientY - rect.top) * 420 / rect.height};
}

function hit(event) {
  const {x, y} = point(event);
  return scenes[current].areas.find(area =>
    x >= area.x && x <= area.x + area.width &&
    y >= area.y && y <= area.y + area.height) ?? null;
}

canvas.addEventListener("pointermove", event => {
  const found = hit(event);
  if (found !== hovered) {
    hovered = found;
    canvas.style.cursor = found ? "pointer" : "default";
    draw();
  }
});
canvas.addEventListener("pointerleave", () => { hovered = null; draw(); });
canvas.addEventListener("click", event => {
  const found = hit(event);
  if (!found) return;
  if (found.target) goTo(found.target);
  else message.textContent = "Bu geçiş bu oynanabilir bölümde henüz yok.";
});
previous.addEventListener("click", () => { const area = areaFor(-1); if (area) goTo(area.target); });
next.addEventListener("click", () => { const area = areaFor(1); if (area) goTo(area.target); });
document.addEventListener("keydown", event => {
  const area = event.key === "ArrowLeft" ? areaFor(-1) :
               event.key === "ArrowRight" ? areaFor(1) : null;
  if (area) { event.preventDefault(); goTo(area.target); }
});

for (const id of ordered) {
  const image = new Image();
  image.onload = draw;
  image.onerror = () => { message.textContent = "Sahne görseli yüklenemedi."; };
  image.src = `assets/${scenes[id].image}`;
  images.set(id, image);
}
draw();
