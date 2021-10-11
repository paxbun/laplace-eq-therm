window.addEventListener("load", () => {
  document.querySelectorAll("span").forEach((elem) => {
    elem.addEventListener("click", () => {
      alert("Hello, world!");
    });
  });
});
