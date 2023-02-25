package coffee

import (
	"fmt"
	"net/http"
	"os"
	"strings"

	"github.com/GoogleCloudPlatform/functions-framework-go/functions"
)

var username = os.Getenv("COFFEE_USER")
var password = os.Getenv("COFFEE_PASSWORD")

func init() {
	functions.HTTP("Coffee", Coffee)
}

// helloHTTP is an HTTP Cloud Function with a request parameter.
func Coffee(w http.ResponseWriter, r *http.Request) {
	authorizationHeader := r.Header.Get("Authorization")

	if authorizationHeader == "" {
		fmt.Fprint(w, "Unauthorized")
		w.WriteHeader(http.StatusUnauthorized)
		return
	}

	headerParts := strings.Split(authorizationHeader, " ")

	if len(headerParts) != 2 || headerParts[0] != "Basic" {
		fmt.Fprint(w, "Unauthorized")
		w.WriteHeader(http.StatusUnauthorized)
		return
	}

	credentials := strings.Split(headerParts[1], ":")

	if len(credentials) != 2 || credentials[0] != username || credentials[1] != password {
		fmt.Fprint(w, "Unauthorized")
		w.WriteHeader(http.StatusUnauthorized)
		return
	}

	fmt.Fprint(w, "Hello, World!")
	return
}
