window.onload = function() {
    // Create a new paragraph element
    var para = document.createElement("p");
    
    // Find the div with the id 'message-container'
    var container = document.getElementById("message-container");

    // Fetch the JSON data
    fetch("http://localhost/json") // Ensure the correct API URL
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
            return response.json(); // Parse JSON data from response
        })
        .then(data => {
            // Loop through the teams and display information
            data.teams.forEach(team => {
                // Create a new div for each team
                var teamDiv = document.createElement("div");

                // Add team name
                var teamName = document.createElement("h3");
                teamName.textContent = `Team: ${team.team_name}`;
                teamDiv.appendChild(teamName);

                // Add other details like abbreviation, city, etc.
                var teamDetails = `
                    <p>Abbreviation: ${team.abbreviation}</p>
                    <p>City: ${team.city}</p>
                    <p>State: ${team.state}</p>
                    <p>Established: ${team.established}</p>
                    <p>Stadium: ${team.stadium}</p>
                    <p>Colors: ${team.colors.join(", ")}</p>
                    <p>Super Bowl Wins: ${team.super_bowl_wins}</p>
                    <p>Head Coach: ${team.head_coach}</p>
                `;
                teamDiv.innerHTML += teamDetails;

                // Append the team div to the container
                container.appendChild(teamDiv);
            });
        })
        .catch(error => {
            console.error("Fetch error:", error);
            para.textContent = "Failed to load data.";
            container.appendChild(para);
        });
};

const hello = async () => {
    try {
        const response = await fetch("http://localhost/raw_msg", {
            headers: { "Accept": "text/raw" }
        });

        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }

        const text = await response.text();
        alert(text);
    } catch (error) {
        alert("Error fetching message: " + error.message);
    }
};
