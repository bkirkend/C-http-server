// This function will populate the div with id 'message-container'
window.onload = function() {
    // Create a new paragraph element
    var para = document.createElement("p");
    
    // Set the text of the paragraph
    para.textContent = "This message was added by JavaScript!";
    
    // Find the div with the id 'message-container'
    var container = document.getElementById("message-container");
    
    // Append the new paragraph to the container
    container.appendChild(para);
};

hello = () => {
  alert("hello");
}

