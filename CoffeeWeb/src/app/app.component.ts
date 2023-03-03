import { Component } from '@angular/core';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'CoffeeWeb';

  isHidden = false
  isGone = false

  toggleIsHidden() {
    this.isHidden = !this.isHidden
  }

  toggleIsGone() {
    this.isGone = !this.isGone
  }

  getSubmenuClasses(): string[] {
    const result = []

    if (this.isHidden) {
      result.push("hidden")
    }

    if (this.isGone) {
      result.push("display-none")
    }

    return result
  }
}
