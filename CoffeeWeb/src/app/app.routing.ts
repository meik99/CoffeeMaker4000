import {RouterModule, Routes} from "@angular/router";
import {CoffeeComponent} from "./coffee/coffee.component";
import {AlarmComponent} from "./alarm/alarm.component";

const routes: Routes = [
  {
    path: '',
    pathMatch: "full",
    redirectTo: 'coffee'
  },
  {
    path: 'coffee',
    component: CoffeeComponent
  },
  {
    path: 'alarm',
    component: AlarmComponent
  }
]

export const AppRouter = RouterModule.forRoot(routes)
